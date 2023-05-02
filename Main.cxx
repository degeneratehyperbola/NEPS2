#include <Windows.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

#include "Globals.hxx"
#include "MemoryTools.hxx"
#include "HookCallbacks.hxx"

// DLL entry point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ulReasonForCall, LPVOID lpReserved)
{
	switch (ulReasonForCall)
	{
	case DLL_PROCESS_ATTACH:
		// Initialize globals
		g_hModule = hModule;
		g_hWnd = FindWindowA(nullptr, "Counter-Strike 2");
		g_pSwapChain = MemorySearch::RelativeToAbsolute<decltype(g_pSwapChain)>(MemorySearch::FindPattern("rendersystemdx11.dll", "66 0F 7F 05 ? ? ? ? 66 0F 7F 0D ? ? ? ? 48 89 35 ? ? ? ?"));

		// Setup ImGui
		ImGui::CreateContext();

		ID3D11Device* pDevice = nullptr;
		ID3D11DeviceContext* pDeviceContext = nullptr;

		g_pSwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&pDevice));
		pDevice->GetImmediateContext(&pDeviceContext);
		
		ImGui_ImplDX11_Init(pDevice, pDeviceContext);
		ImGui_ImplWin32_Init(g_hWnd);

		// Setup our hooking method
		if (!HookManager::Setup())
			return FALSE;

		// Hook interesting functions
		auto pSwapChainVtable = GET_VTABLE(g_pSwapChain);
		g_hmPresent = HookManager(pSwapChainVtable[8], Callbacks::Present);
		g_hmResizeBuffers = HookManager(pSwapChainVtable[13], Callbacks::ResizeBuffers);

		g_pOriginalWndProc = (WNDPROC)SetWindowLongPtrW(g_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Callbacks::WndProc));

		g_hmPresent.Hook();
		g_hmResizeBuffers.Hook();

		break;
	case DLL_PROCESS_DETACH:
		// Unhook hooked hooks
		SetWindowLongPtrW(g_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(g_pOriginalWndProc));
		g_hmResizeBuffers.Unhook();
		g_hmPresent.Unhook();
		
		// Cleanup after ourselves
		HookManager::Cleanup();

		// Cleanup ImGui
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		break;
	}
	return TRUE;
}