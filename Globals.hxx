#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <HookManager.hxx>

extern HMODULE g_hModule; // General globals | Handle to this DLL
extern HWND g_hWnd; // General globals | Handle to the game's window

extern bool g_bIsUsingVulkan; // Rendering related globals
extern ID3D11Device* g_pD3DDevice; // Rendering related globals | Hijacked D3D device interface (reference count not incremented)

extern WNDPROC g_pOriginalWndProc; // Hook related globals | Callbacks::WndProc
extern HookManager g_hkPresent; // Hook related globals | Callbacks::Present
extern HookManager g_hkResizeBuffers; // Hook related globals | Callbacks::ResizeBuffers
