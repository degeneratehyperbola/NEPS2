#include <Windows.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>
#include <MemoryTools.hxx>

#include "Globals.hxx"
#include "HookCallbacks.hxx"
#include "Helpers.hxx"

// Scan for an essential memory address, otherwise self-destruct
#define FIND_PATTERN_IMPORTANT(variableName, moduleName, pattern, offsetFromInitialResult) \
	uintptr_t variableName = MemorySearch::FindPatternInModule(moduleName, pattern); \
	if (!variableName) \
	{ \
		NEPS::Error("Pattern not found!", "%s.dll | %s", moduleName, pattern); \
		return FALSE; \
	} \
	variableName = *MemorySearch::RelativeToAbsolute<uintptr_t*>(variableName + offsetFromInitialResult);

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

		if (!g_hWnd)
		{
			NEPS::Error("Failed to find game's window!", "Expected name \"%s\"", "Counter-Strike 2");
			return FALSE;
		}

		g_bIsUsingVulkan = GetModuleHandleA("rendersystemvulkan.dll");

		// Setup ImGui
		ImGui::CreateContext();

		ImGui_ImplWin32_Init(g_hWnd);

		// Setup our hooking method
		if (!HookManager::Setup())
		{
			NEPS::Error("Hook manager initializaation failed!", "Line %u", __LINE__ - 2);
			return FALSE;
		}

		// Hook interesting functions
		g_pOriginalWndProc = (WNDPROC)SetWindowLongPtrW(g_hWnd, GWLP_WNDPROC, (LONG_PTR)Callbacks::WndProc);

		if (g_bIsUsingVulkan)
		{
			// TODO Vulkan support
		}
		else
		{
			FIND_PATTERN_IMPORTANT(fnPresent, "gameoverlayrenderer64", "4C 8D 05 ? ? ? ? 41 B9 ? ? ? ? 48 8D 15 ? ? ? ? E8 ? ? ? ? 48 8B 4F 50", 3);
			FIND_PATTERN_IMPORTANT(fnResizeBuffers, "gameoverlayrenderer64", "48 8B 4F 68 4C 8D 05", 7);

			g_hkPresent = HookManager(fnPresent, Callbacks::Present);
			g_hkResizeBuffers = HookManager(fnResizeBuffers, Callbacks::ResizeBuffers);

			g_hkPresent.Hook();
			g_hkResizeBuffers.Hook();
		}

		break;
	}
	case DLL_PROCESS_DETACH:
	{
		// Unhook hooked hooks <3
		if (g_pOriginalWndProc)
			SetWindowLongPtrW(g_hWnd, GWLP_WNDPROC, (LONG_PTR)g_pOriginalWndProc);
		
		g_hkResizeBuffers.Unhook();
		g_hkPresent.Unhook();

		// Cleanup after ourselves
		HookManager::Cleanup();

		// Cleanup ImGui
		if (g_bIsUsingVulkan)
			; // TODO Vulkan support
		else
			ImGui_ImplDX11_Shutdown();

		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		break;
	}
	}
	return TRUE;
}
