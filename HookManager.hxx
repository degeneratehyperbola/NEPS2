#pragma once

#include <minhook/MinHook.h>

class HookManager
{
public:
	static bool Setup()
	{
		if (MH_Initialize() != MH_OK)
			return false;
	}

	static void Cleanup()
	{
		MH_Uninitialize();
	}

	template<typename T, typename C>
	HookManager(T* pTarget, C* pCallback)
		: m_pTarget{pTarget}
		, m_pCallback{pCallback}
		, m_pTrampoline{nullptr}
	{
		// Use Hook() to initialize
	}

	~HookManager()
	{
		Unhook();
	}

	bool Hook()
	{
		if (MH_CreateHook(m_pTarget, m_pCallback, &m_pTrampoline) != MH_OK)
			return false;

		if (MH_EnableHook(m_pTarget) != MH_OK)
			return false;

		return true;
	}

	void Unhook()
	{
		if (m_pTrampoline != nullptr)
		{
			MH_DisableHook(m_pTarget);
			MH_RemoveHook(m_pTarget);
			m_pTrampoline = nullptr;
		}
	}

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
