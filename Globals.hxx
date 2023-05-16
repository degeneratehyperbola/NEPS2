#pragma once

#include <PCH.h>

inline HMODULE g_hModule; // General globals | Handle to this DLL
inline HWND g_hWnd; // General globals | Handle to the game's window

inline bool g_bIsUsingVulkan; // Rendering related globals
inline bool g_bImGuiInitialized; // Rendering related globals | Is ImGui backend initialized

inline WNDPROC g_pOriginalWndProc; // Hooking related globals | Callbacks::WndProc
inline HookManager g_hkPresent; // Hooking related globals | Callbacks::Present
inline HookManager g_hkResizeBuffers; // Hooking related globals | Callbacks::ResizeBuffers
