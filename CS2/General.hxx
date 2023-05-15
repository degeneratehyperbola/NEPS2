#pragma once

#include "SDK/interfaces/InputSystem.hxx"
#include "SDK/interfaces/EngineClient.hxx"

namespace CS2
{
	void SetupInterfaces();

	void SetRelativeMouseMode(bool enabled);
	void EnableWindowPolling(bool enabled);
	void SetMousePos(int x, int y);

	inline IInputSystem* InputSystem;
	inline IEngineClient* EngineClient;
}
