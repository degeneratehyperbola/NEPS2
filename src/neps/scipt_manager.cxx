#include "script_manager.hxx"

#include "globals.hxx"


static bool s_bInitialized = false; // Keep track of interpreter initialization


bool ScriptManager::Setup()
{
	s_bInitialized = true;

	ScanDirectory();

	return true;
}

void ScriptManager::Cleanup()
{
	if (s_bInitialized)
	{
		s_bInitialized = false;
	}
}


void ScriptManager::ScanDirectory()
{
	TrackedScripts.clear();

	for (const auto& entry : fs::directory_iterator(g_dirScripts))
	{
		if (fs::is_regular_file(entry) && entry.path().extension() == ".ass")
			TrackedScripts.push_back(Script{ entry.path() });
	}
}

void ScriptManager::Script::Load()
{
	if (!fs::exists(g_dirScripts / this->Path)) return;

	this->Loaded = true;
}

void ScriptManager::Script::Unload()
{
	this->Loaded = false;
}
