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
		py::dict Scope{ };
		bool Loaded = false;
	};


	bool Setup();
	void Cleanup();
	void ScanDirectory();
	void LoadScript(Script& script);
	void Unload(Script& script);


	inline std::list<Script> TrackedScripts;
}
