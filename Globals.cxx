#include "Globals.hxx"

HMODULE g_hModule = nullptr;
HWND g_hWnd = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;

HookManager g_hmPresentHook;
