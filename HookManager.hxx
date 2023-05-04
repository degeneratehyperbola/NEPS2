#pragma once

#include <minhook/MinHook.h>

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

	template<typename T, typename C>
	HookManager(T* pTarget, C* pCallback) : m_pTarget{ (void*)pTarget }, m_pCallback{ (void*)pCallback }, m_pTrampoline{ nullptr }
	{
		// Use Hook() to initialize
	}

	~HookManager() { Unhook(); }

	bool Hook()
	{
		if (!m_pTarget || !m_pCallback) return false;

		if (MH_CreateHook(m_pTarget, m_pCallback, &m_pTrampoline) != MH_OK)
			return false;

		if (MH_EnableHook(m_pTarget) != MH_OK)
			return false;

		return true;
	}

	void Unhook()
	{
		if (IsHooked())
		{
			MH_DisableHook(m_pTarget);
			MH_RemoveHook(m_pTarget);
			m_pTrampoline = nullptr;
		}
	}

	bool IsHooked() { return m_pTrampoline; }

	template<typename Ret, typename... Args>
	Ret CallOriginal(Args... args)
	{
		auto originalFunction = reinterpret_cast<Ret(*)(Args...)>(m_pTrampoline);
		return originalFunction(args...);
	}

private:
	void* m_pTarget;
	void* m_pCallback;
	void* m_pTrampoline;
};
