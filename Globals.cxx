#include "Globals.hxx"

HMODULE g_hModule = nullptr;
HWND g_hWnd = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;

WNDPROC g_pOriginalWndProc;
HookManager g_hmPresent;
HookManager g_hmResizeBuffers;
