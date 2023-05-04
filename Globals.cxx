#include "Globals.hxx"

HMODULE g_hModule = nullptr;
HWND g_hWnd = nullptr;

bool g_bIsUsingVulkan;
IDXGISwapChain* g_pSwapChain = nullptr;

WNDPROC g_pOriginalWndProc;
HookManager g_hkPresent;
HookManager g_hkResizeBuffers;
