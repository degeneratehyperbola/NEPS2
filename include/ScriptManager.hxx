#pragma once


#include <filesystem>

// ScriptManager is used to load and track scripts as well as providing them with an individual context and exposing the API
// Currently used interpreter: Python3
class ScriptManager
{
public:
	static bool Setup()
	{
		// Init python interpreter here
	}

	static void Cleanup()
	{
		// Uninit python interpreter here
	}

private:
	std::filesystem::path m_Path;
	bool m_bIsLoaded;
};
