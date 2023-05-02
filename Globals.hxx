#pragma once

#include <Windows.h>
#include <d3d11.h>

#include "HookManager.hxx"

extern HMODULE g_hModule;
extern HWND g_hWnd;
extern IDXGISwapChain* g_pSwapChain;

extern HookManager g_hmPresent;
extern HookManager g_hmResizeBuffers;
