#pragma once

#include <pch.hpp>

#include "interfaces/input_system.hxx"
#include "interfaces/engine_client.hxx"


namespace CS2
{
	void SetupInterfaces();

	void SetRelativeMouseMode(bool enabled);
	void SetMouseCapture(bool enabled);
	void WarpMouseInWindow(int x, int y);
	void GetWindowSize(int* w, int* h);

	inline IInputSystem* InputSystem;
	inline IEngineClient* EngineClient;
}
