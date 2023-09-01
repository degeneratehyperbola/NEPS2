#pragma once

#include <PCH.hpp>


// ScriptManager is used to load and track scripts as well as providing them with an individual context and exposing the API
// Currently used interpreter: Python3
namespace ScriptManager
{
	struct Script
	{
		std::filesystem::path Path;
		bool Loaded;
	};
	

	void Setup()
	{
		pybind11::initialize_interpreter();
	}

	static void Cleanup()
	{
		pybind11::finalize_interpreter();
	}
}
