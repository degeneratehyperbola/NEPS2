#pragma once

#include <Windows.h>
#include <d3d11.h>

#include "HookManager.hxx"

namespace Hooks
{
	extern HMODULE g_hModule;
	extern HookManager g_hmPresentHook;
}

namespace Memory
{
	extern void* g_pPresent;
}
