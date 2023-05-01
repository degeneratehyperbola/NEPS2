#include <Windows.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>

#include "Globals.hxx"
#include "MemorySearch.hxx"
#include "HookCallbacks.hxx"

// DLL entry point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Hooks::g_hModule = hModule;
		// Memory::g_pPresent = MemorySearch::FindPattern();

		if (!HookManager::Setup())
			return FALSE;

		ImGui::CreateContext();

		Hooks::g_hmPresentHook = HookManager(Memory::g_pPresent, PresentCallback);

		Hooks::g_hmPresentHook.Hook();

		break;
	case DLL_PROCESS_DETACH:
		Hooks::g_hmPresentHook.Unhook();
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();
		HookManager::Cleanup();
		break;
	}
	return TRUE;
}