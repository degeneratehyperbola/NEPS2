#pragma once

#include <minhook/MinHook.h>

// HookManager is used for hooking and managing a single target function
class HookManager
{
public:
	static bool Setup()
	{
		if (MH_Initialize() != MH_OK)
			return false;

		return true;
	}

	static void Cleanup()
	{
		MH_Uninitialize();
	}


	HookManager() = default;

	// HookManager is used for hooking and managing a single target function
	template<typename T, typename C>
	HookManager(T fnTarget, C fnCallback)
	{
		m_fnTarget = (void*)fnTarget;
		m_fnCallback = (void*)fnCallback;
		m_fnOriginal = nullptr;
	}

	~HookManager() { Unhook(); }


	bool Hook()
	{
		if (!m_fnTarget || !m_fnCallback || IsHooked()) return false;

		if (MH_CreateHook(m_fnTarget, m_fnCallback, &m_fnOriginal) != MH_OK)
			return false;

		if (MH_EnableHook(m_fnTarget) != MH_OK)
			return false;

		return true;
	}

	void Unhook()
	{
		if (IsHooked())
		{
			MH_DisableHook(m_fnTarget);
			MH_RemoveHook(m_fnTarget);
			m_fnOriginal = nullptr;
		}
	}

	bool IsHooked() { return m_fnOriginal; }

	template<typename Ret, typename... Args>
	Ret CallOriginal(Args... args)
	{
		auto originalFunction = reinterpret_cast<Ret(*)(Args...)>(IsHooked() ? m_fnOriginal : m_fnTarget);
		return originalFunction(args...);
	}


private:
	void* m_fnTarget;
	void* m_fnCallback;
	void* m_fnOriginal;
};
