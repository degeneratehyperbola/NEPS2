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
	{
		// Initialize globals
		g_hModule = hModule;
		g_hWnd = FindWindowA(nullptr, "Counter-Strike 2");

		g_bIsUsingVulkan = GetModuleHandleA("rendersystemvulkan.dll");

		if (g_bIsUsingVulkan)
		{
			// TODO Vulkan support
		} else
		{
			g_pD3DDevice = *MemorySearch::RelativeToAbsolute<ID3D11Device**>(MemorySearch::FindPattern("rendersystemdx11", "48 89 1D ? ? ? ? 48 89 3D") + 10);
		}

		// Setup ImGui
		ImGui::CreateContext();

		if (g_bIsUsingVulkan)
		{
			// TODO Vulkan support
		} else
		{
			ID3D11DeviceContext* pDeviceContext = nullptr;
			g_pD3DDevice->GetImmediateContext(&pDeviceContext);

			ImGui_ImplDX11_Init(g_pD3DDevice, pDeviceContext);
		}
		
		ImGui_ImplWin32_Init(g_hWnd);

		// Setup our hooking method
		if (!HookManager::Setup())
			return FALSE;

		// Hook interesting functions
		g_pOriginalWndProc = (WNDPROC)SetWindowLongPtrW(g_hWnd, GWLP_WNDPROC, (LONG_PTR)Callbacks::WndProc);

		void* pPresent = *MemorySearch::RelativeToAbsolute<void**>(MemorySearch::FindPattern("gameoverlayrenderer64", "4C 8D 05 ? ? ? ? 41 B9 ? ? ? ? 48 8D 15 ? ? ? ? E8 ? ? ? ? 48 8B 4F 50") + 3);
		void* pResizeBuffers = *MemorySearch::RelativeToAbsolute<void**>(MemorySearch::FindPattern("gameoverlayrenderer64", "48 8B 4F 68 4C 8D 05") + 7);
		g_hkPresent = HookManager(pPresent, Callbacks::Present);
		g_hkResizeBuffers = HookManager(pResizeBuffers, Callbacks::ResizeBuffers);

		g_hkPresent.Hook();
		g_hkResizeBuffers.Hook();

		break;
	}
	case DLL_PROCESS_DETACH:
	{
		// Unhook hooked hooks
		SetWindowLongPtrW(g_hWnd, GWLP_WNDPROC, (LONG_PTR)g_pOriginalWndProc);
		g_hkResizeBuffers.Unhook();
		g_hkPresent.Unhook();

		// Cleanup after ourselves
		HookManager::Cleanup();

		// Cleanup ImGui
		if (g_bIsUsingVulkan)
			{} // TODO Vulkan support
		else
			ImGui_ImplDX11_Shutdown();

		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		break;
	}
	}
	return TRUE;
}
