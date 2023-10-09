#pragma once

#include "Globals.hxx"
#include "InputSystem.hxx"
#include "HookCallbacks.hxx"
#include "GUI.hxx"
#include "ScriptManager.hxx"


// Basic NEPS functionality
namespace NEPS
{
	void Unload();
	bool FatalError(const char* title, const char* contentFmt, ...);
}
