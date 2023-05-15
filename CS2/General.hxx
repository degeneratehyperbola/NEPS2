#pragma once

#include "SDK/interfaces/InputSystem.hxx"
#include "SDK/interfaces/EngineClient.hxx"

namespace CS2
{
	void SetupInterfaces();

	void SetRelativeMouseMode(bool enabled);
	void SetMouseCapture(bool enabled);
	void WarpMouseInWindow(int x, int y);

	inline IInputSystem* InputSystem;
	inline IEngineClient* EngineClient;
}
