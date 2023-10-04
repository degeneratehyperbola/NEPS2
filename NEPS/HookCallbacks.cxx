#include "HookCallbacks.hxx"

#include <PCH.hpp>

#include "CS2/General.hxx"
#include "General.hxx"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);


// DirectX11 | Called each render frame
HRESULT WINAPI Callbacks::Present(IDXGISwapChain* pSwapChain, UINT syncInterval, UINT flags)
{
	if (!g_bImGuiInitialized)
	{
		ID3D11Device* pD3DDevice = nullptr;
		ID3D11DeviceContext* pDeviceContext = nullptr;
		pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pD3DDevice);
		pD3DDevice->GetImmediateContext(&pDeviceContext);

		ImGui_ImplDX11_Init(pD3DDevice, pDeviceContext);

		auto& io = ImGui::GetIO();
		// We do not want to create any files inside the game folder (OwO') !!!
		io.IniFilename = nullptr;
		io.LogFilename = nullptr;

		g_bImGuiInitialized = true;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	GUI::Render();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return g_hkPresent.CallOriginal<HRESULT>(pSwapChain, syncInterval, flags);
}

// DirectX11 | Called when the game's window is resized
HRESULT WINAPI Callbacks::ResizeBuffers(IDXGISwapChain* pSwapChain, UINT bufferCount, UINT w, UINT h, DXGI_FORMAT newFormat, UINT flags)
{
	if (g_bImGuiInitialized)
		ImGui_ImplDX11_InvalidateDeviceObjects(); // Device objects are recreated at the next ImGui_ImplDX11_NewFrame call

	return g_hkResizeBuffers.CallOriginal<HRESULT>(pSwapChain, bufferCount, w, h, newFormat, flags);
}

// Calls are event-based (e.g. keypress, clipboard action)
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
			NEPS::InputEvent::KeyDown(wParam);

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
					CS2::EngineClient->GetScreenSize(w, h);
					CS2::WarpMouseInWindow(w / 2, h / 2);
				}
			}
		}

		if (uMsg == WM_KEYUP)
			NEPS::InputEvent::KeyUp(wParam);

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
