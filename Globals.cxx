#include "Globals.hxx"

HMODULE g_hModule;
HWND g_hWnd;

bool g_bIsUsingVulkan;
bool g_bImGuiInitialized;

WNDPROC g_pOriginalWndProc;
HookManager g_hkPresent;
HookManager g_hkResizeBuffers;
