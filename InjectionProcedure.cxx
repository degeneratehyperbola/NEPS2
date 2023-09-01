#include <PCH.hpp>

#include "CS2/General.hxx"
#include "NEPS/General.hxx"


// Scan for an essential memory address, otherwise self-destruct
#define FIND_IMPORTANT(variableName, moduleName, pattern, offset) \
	uintptr_t variableName = MemorySearch::FindPattern(moduleName, pattern) + offset; \
	if (!variableName) \
		return NEPS::Error("Pattern or module not found", "Could not find\n%s.dll | %s", moduleName, #variableName); \

// Same as above but we are searching for a relative pointer
#define FIND_IMPORTANT_RELATIVE(variableName, moduleName, pattern, offsetFromInitialResult) \
	FIND_IMPORTANT(variableName, moduleName, pattern, offsetFromInitialResult); \
	variableName = *MemorySearch::RelativeToAbsolute<uintptr_t*>(variableName);


// DLL entry point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ulReasonForCall, LPVOID lpReserved)
{
	switch (ulReasonForCall)
	{
	case DLL_PROCESS_ATTACH:
	{
		// Initialize globals
		g_hModule = hModule;
		g_hWnd = FindWindowA("SDL_app", nullptr);

		if (!g_hWnd)
			return NEPS::Error("Could not find game window", "Expected class name \"%s\"", "SDL_app");

		g_bIsUsingVulkan = GetModuleHandleA("rendersystemvulkan");

		// Find various game interfaces and functions
		CS2::SetupInterfaces();

		// Setup ImGui
		if (!ImGui::CreateContext())
			return NEPS::Error("Could not create ImGui context", STRINGIFY(ImGui::CreateContext));

		if (!ImGui_ImplWin32_Init(g_hWnd))
			return NEPS::Error("Could not initialize ImGui windows implementation", STRINGIFY(ImGui_ImplWin32_Init));

		// Setup our hooking method
		if (!HookManager::Setup())
			return NEPS::Error("Could not setup hook manager", STRINGIFY(HookManager::Setup));

		// Hook interesting functions
		if (!(g_pOriginalWndProc = (WNDPROC)SetWindowLongPtrW(g_hWnd, GWLP_WNDPROC, (LONG_PTR)Callbacks::WndProc)))
			return NEPS::Error("Could not hook WndProc", STRINGIFY(SetWindowLongPtrW));

		if (g_bIsUsingVulkan)
			// TODO Vulkan support
			return NEPS::Error("Vulkan is not yet supported", STRINGIFY(g_bIsUsingVulkan) " is true");
		else
		{
			FIND_IMPORTANT(fnPresent, "gameoverlayrenderer64", "48 89 6C 24 18 48 89 74 24 20 41 56 48 83 EC 20 41 8B E8", 0);
			FIND_IMPORTANT_RELATIVE(fnResizeBuffers, "gameoverlayrenderer64", "48 8B 4F 68 4C 8D 05", 7);

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
		if (g_bImGuiInitialized)
		{
			if (g_bIsUsingVulkan)
				; // TODO Vulkan support
			else
				ImGui_ImplDX11_Shutdown();
		}

		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		break;
	}
	}
	return TRUE;
}
