#include "HookCallbacks.hxx"

#include <Windows.h>
#include <d3d11.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

#include "CS2/General.hxx"

#include "Globals.hxx"
#include "Helpers.hxx"
#include "GUI.hxx"

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
		
		io.BackendFlags &= ~ImGuiBackendFlags_HasMouseCursors; // Disable mouse cursor changes as they're handled by the game

		g_bImGuiInitialized = true;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	GUI::Render();

	// Show mouse cursor when GUI is open, otherwise restore to previous state
	if (GUI::IsOpen())
	{
		CS2::SetRelativeMouseMode(false);
		CS2::EnableWindowPolling(false);
	}
	else if (CS2::InputSystem->IsRelativeMouseMode())
	{
		CS2::SetRelativeMouseMode(true);
		CS2::EnableWindowPolling(true);
		int w, h;
		CS2::EngineClient->GetScreenSize(w, h);
		CS2::SetMousePos(w / 2, h / 2);
	}

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

	if (uMsg == WM_KEYDOWN && wParam == VK_END)
		NEPS::Unload();

	// Disable all input if GUI is open
	if (GUI::IsOpen())
	{
		switch (uMsg)
		{
			case WM_MOUSEMOVE:
			case WM_NCMOUSEMOVE:
			case WM_MOUSELEAVE:
			case WM_NCMOUSELEAVE:
			case WM_LBUTTONDOWN:
			case WM_LBUTTONDBLCLK:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONDBLCLK:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONDBLCLK:
			case WM_XBUTTONDOWN:
			case WM_XBUTTONDBLCLK:
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			case WM_XBUTTONUP:
			case WM_MOUSEWHEEL:
			case WM_MOUSEHWHEEL:
			case WM_KEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_SETFOCUS:
			case WM_KILLFOCUS:
			case WM_CHAR:
			case WM_DEVICECHANGE:
				return 1;
		}
	}

	return CallWindowProcW(g_pOriginalWndProc, hWnd, uMsg, wParam, lParam);
}
