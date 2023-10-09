#pragma once

#include <PCH.hpp>


inline HMODULE g_hModule; // General globals | Handle to this DLL
inline HWND g_hWnd; // General globals | Handle to the game's window
inline fs::path g_dirProfile; // General globals | Profile directory path (configs and scripts)
inline fs::path g_dirConfigs; // General globals | Config directory path 
inline fs::path g_dirScripts; // General globals | Script directory path

inline bool g_bIsUsingVulkan; // Rendering related globals
inline bool g_bImGuiInitialized; // Rendering related globals | Is ImGui backend initialized

inline WNDPROC g_pOriginalWndProc = nullptr; // Hooking related globals | Callbacks::WndProc
inline HookManager g_hkPresent; // Hooking related globals | Callbacks::Present
inline HookManager g_hkResizeBuffers; // Hooking related globals | Callbacks::ResizeBuffers
