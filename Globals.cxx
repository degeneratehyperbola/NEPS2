#include "Globals.hxx"

HMODULE g_hModule;
HWND g_hWnd;

bool g_bIsUsingVulkan;
ID3D11Device* g_pD3DDevice;

WNDPROC g_pOriginalWndProc;
HookManager g_hkPresent;
HookManager g_hkResizeBuffers;
