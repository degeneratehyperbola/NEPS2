#pragma once

#include "globals.hxx"
#include "input_system.hxx"
#include "hook_callbacks.hxx"
#include "gui.hxx"
#include "script_manager.hxx"


// Basic NEPS functionality
namespace NEPS
{
	void Unload();
	bool FatalError(const char* title, const char* contentFmt, ...);
}
