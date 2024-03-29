#include "hook_callbacks.hxx"

#include <pch.hpp>

#include "cs2/general.hxx"
#include "general.hxx"


// DirectX11 | Dedicated render target to isolate our rendering from game's
static ID3D11RenderTargetView* s_pRenderTarget;


// DirectX11 | Called each render frame
HRESULT WINAPI Callbacks::Present(IDXGISwapChain* pSwapChain, UINT syncInterval, UINT flags)
{
	ID3D11Device* pD3DDevice = nullptr;
	pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pD3DDevice);
	
	ID3D11DeviceContext* pDeviceContext = nullptr;
	pD3DDevice->GetImmediateContext(&pDeviceContext);

	// WndProc hoking nd some ImGui setup needs to be done in the game's thread
	if (!g_bImGuiInitialized)
	{
		if (!ImGui_ImplDX11_Init(pD3DDevice, pDeviceContext))
			return NEPS::FatalError("Initialization", "Could not initialize ImGui DX11 frontend");

		// We do not want to create any files inside the game folder (OwO') !!!
		auto& io = ImGui::GetIO();
		io.IniFilename = nullptr;
		io.LogFilename = nullptr;

		g_bImGuiInitialized = true;
	}

	if (!g_pOriginalWndProc)
	{
		DXGI_SWAP_CHAIN_DESC desc;
		pSwapChain->GetDesc(&desc);
		g_hWnd = desc.OutputWindow;

		if (!ImGui_ImplWin32_Init(g_hWnd))
			return NEPS::FatalError("Initialization", "Could not initialize ImGui backend");

		g_pOriginalWndProc = (WNDPROC)SetWindowLongPtrW(g_hWnd, GWLP_WNDPROC, (LONG_PTR)Callbacks::WndProc);
	}

	if (!s_pRenderTarget)
	{
		ID3D11Texture2D* backBuffer;
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

		D3D11_RENDER_TARGET_VIEW_DESC desc;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

		pD3DDevice->CreateRenderTargetView(backBuffer, &desc, &s_pRenderTarget);

		backBuffer->Release();
	}

	// Set to draw onto our render target
	// It is necessary to do this before creating DX11 related backend objects in ImGui
	// (ImGui_ImplDX11_NewFrame and subsequently ImGui_ImplDX11_CreateDeviceObjects)
	pDeviceContext->OMSetRenderTargets(1, &s_pRenderTarget, nullptr);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	GUI::Render();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return g_hkPresent.CallOriginal<HRESULT>(pSwapChain, syncInterval, flags);
}

// DirectX11 | Called when the resolution changes and device needs resetting
HRESULT WINAPI Callbacks::ResizeBuffers(IDXGISwapChain* pSwapChain, UINT bufferCount, UINT w, UINT h, DXGI_FORMAT newFormat, UINT flags)
{
	if (s_pRenderTarget)
	{
		s_pRenderTarget->Release();
		s_pRenderTarget = nullptr;
	}

	if (g_bImGuiInitialized)
		ImGui_ImplDX11_InvalidateDeviceObjects(); // Device objects are recreated at the next ImGui_ImplDX11_NewFrame call

	return g_hkResizeBuffers.CallOriginal<HRESULT>(pSwapChain, bufferCount, w, h, newFormat, flags);
}

// Windows | No header with this declaration for some reason
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

// Windows | Calls are event-based (e.g. keypress, clipboard action)
LRESULT WINAPI Callbacks::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

	// SDL3 unimplementation TwT
	// SDL3 begone!! Unimplement!!1!
	// We capture some messages :3
	switch (uMsg)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_CHAR:

	case WM_SETFOCUS:
	case WM_KILLFOCUS:
	case WM_DEVICECHANGE:

	case WM_MOUSEMOVE:
	case WM_MOUSELEAVE:
	case WM_NCMOUSEMOVE:
	case WM_NCMOUSELEAVE:

	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:

	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:

	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:

	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_XBUTTONDBLCLK:

	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:

	case WM_SETCURSOR:
		// Our input handling
		if (uMsg == WM_KEYDOWN && !(lParam & (1 << 30)))
		{
			InputEvent::KeyDown(wParam);

			if (wParam == VK_END)
				NEPS::Unload();
			// Menu toggle buttons
			else if (wParam == VK_INSERT || wParam == VK_DELETE)
			{
				GUI::IsOpen = !GUI::IsOpen;

				// Place cursor in the center for convenience
				if (GUI::IsOpen && CS2::InputSystem->WantToCaptureMouse())
				{
					int w, h;
					CS2::GetWindowSize(&w, &h);
					CS2::WarpMouseInWindow(w / 2, h / 2);
				}
			}
		}

		if (uMsg == WM_KEYUP)
			InputEvent::KeyUp(wParam);

		// Unlock cursor whenever we need it, otherwise restore polling
		if (GUI::IsOpen)
		{
			CS2::SetRelativeMouseMode(false);
			CS2::SetMouseCapture(false);
		}
		else if (CS2::InputSystem->WantToCaptureMouse())
		{
			CS2::SetRelativeMouseMode(true);
			CS2::SetMouseCapture(true);
		}

		// Allow keyboard inputs unless we are capturing them
		if (WM_KEYLAST >= uMsg && uMsg >= WM_KEYFIRST && !ImGui::GetIO().WantCaptureKeyboard)
			break;

		// Skip game's input handling and cursor changing if GUI is open
		if (GUI::IsOpen) return ERROR_SUCCESS;

		break;
	}

	return CallWindowProcW(g_pOriginalWndProc, hWnd, uMsg, wParam, lParam);
}
