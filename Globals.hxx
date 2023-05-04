#pragma once

#include <Windows.h>
#include <d3d11.h>

#include "HookManager.hxx"

extern HMODULE g_hModule;
extern HWND g_hWnd;

extern bool g_bIsUsingVulkan;
extern IDXGISwapChain* g_pSwapChain;

extern WNDPROC g_pOriginalWndProc;
extern HookManager g_hkPresent;
extern HookManager g_hkResizeBuffers;
