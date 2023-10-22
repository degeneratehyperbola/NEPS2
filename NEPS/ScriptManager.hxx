#pragma once

#include <PCH.hpp>


// ScriptManager is used to load and track scripts as well as providing them with an individual context
// Currently used interpreter: None
namespace ScriptManager
{
	// Script structure tracks a single script, like its path and global scope
	struct Script
	{
		fs::path Path;
		bool Loaded = false;

		void Load();
		void Unload();
	};


	bool Setup();
	void Cleanup();
	void ScanDirectory();


	inline std::vector<Script> TrackedScripts;
}
