#include "HookCallbacks.hxx"

#include <Windows.h>
#include <d3d11.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

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
		io.BackendFlags &= ~ImGuiBackendFlags_HasMouseCursors; // Disable mouse cursor changes as they're handled by the game

		g_bImGuiInitialized = true;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	#ifdef _DEBUG
	GUI::RenderDebugWindow();
	#endif
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

	if (uMsg == WM_KEYDOWN && wParam == VK_END)
		NEPS::Unload();

	return CallWindowProcW(g_pOriginalWndProc, hWnd, uMsg, wParam, lParam);
}
