#pragma once

#include <PCH.hpp>


// ScriptManager is used to load and track scripts as well as providing them with an individual context
// Currently used interpreter: Python3
namespace ScriptManager
{
	// Script structure tracks a single script, like its path and global scope
	struct Script
	{
		fs::path Path;
		std::unique_ptr<py::dict> Scope = nullptr;
		bool Loaded = false;

		void Load();
		void Unload();
	};

	class AcquireGIL
	{
	public:
		AcquireGIL()
		{
			m_State = PyGILState_Ensure();
		}

		~AcquireGIL()
		{
			PyGILState_Release(m_State);
		}

	private:
		PyGILState_STATE m_State;
	};


	bool Setup();
	void Cleanup();
	void ScanDirectory();


	inline std::vector<Script> TrackedScripts;
}
