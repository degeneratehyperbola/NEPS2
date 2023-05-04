#pragma once

#include <Windows.h>
#include <d3d11.h>

#include "HookManager.hxx"

extern HMODULE g_hModule;
extern HWND g_hWnd;

extern bool g_bIsUsingVulkan;
extern ID3D11Device* g_pD3DDevice;

extern WNDPROC g_pOriginalWndProc;
extern HookManager g_hkPresent;
extern HookManager g_hkResizeBuffers;
