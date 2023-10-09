#include "ScriptManager.hxx"

#include "Globals.hxx"


bool ScriptManager::Setup()
{
	try { py::initialize_interpreter(); }
	catch (std::exception& e) { return false; }

	ScanDirectory();

	Initialized = true;
	return true;
}

void ScriptManager::Cleanup()
{
	if (Initialized)
		py::finalize_interpreter();
}

void ScriptManager::ScanDirectory()
{
	TrackedScripts.clear();

	for (const auto& entry : fs::directory_iterator(g_dirScripts))
	{
		if (fs::is_regular_file(entry) && entry.path().extension() == ".py")
			TrackedScripts.push_back(Script{ entry.path() });
	}
}

void ScriptManager::LoadScript(Script& script)
{
	script.Scope = std::make_unique<py::dict>();
	py::eval_file(script.Path.string().c_str(), *script.Scope);
	script.Loaded = true;
}

void ScriptManager::Unload(Script& script)
{
	script.Scope.reset();
	script.Loaded = false;
}
