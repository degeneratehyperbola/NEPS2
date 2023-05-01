#include "Globals.hxx"

namespace Hooks
{
	HMODULE g_hModule = nullptr;
	HookManager g_hmPresentHook;
}

namespace Memory
{
	void* g_pPresent;
}
