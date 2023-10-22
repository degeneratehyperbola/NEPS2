#include "ScriptManager.hxx"

#include "Globals.hxx"


static bool s_bInitialized = false; // Keep track of interpreter initialization
static PyThreadState* s_ThreadState = nullptr; // The GIL TwT


bool ScriptManager::Setup()
{
	try { py::initialize_interpreter(); }
	catch (std::exception& e) { return false; }
	s_ThreadState = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
	s_bInitialized = true;

	ScanDirectory();

	return true;
}

void ScriptManager::Cleanup()
{
	if (s_bInitialized)
	{
		PyEval_RestoreThread(s_ThreadState); // Py_END_ALLOW_THREADS
		py::finalize_interpreter();
		s_bInitialized = false;
	}
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

void ScriptManager::Script::Load()
{
	if (!fs::exists(g_dirScripts / this->Path)) return;

	this->Scope = std::make_unique<py::dict>();
	py::eval_file(this->Path.string().c_str(), *this->Scope);
	this->Loaded = true;
}

void ScriptManager::Script::Unload()
{
	this->Scope.reset();
	this->Loaded = false;
}
