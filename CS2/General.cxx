#include "General.hxx"

#include <Windows.h>

#include <MemoryTools.hxx>

void CS2::SetRelativeMouseMode(bool enabled)
{
	auto fn = EXTERNAL_PROC("SDL2", "SDL_SetRelativeMouseMode", int, (bool));
	if (fn) fn(enabled);
}

void CS2::EnableWindowPolling(bool enabled)
{
	auto fn = EXTERNAL_PROC("SDL2", "SDL_SetWindowGrab", void, (void*, bool));
	if (fn) fn(InputSystem->GetSDLWindow(), enabled);
}

void CS2::SetMousePos(int x, int y)
{
	auto fn = EXTERNAL_PROC("SDL2", "SDL_WarpMouseInWindow", void, (void*, int, int));
	if (fn) fn(InputSystem->GetSDLWindow(), x, y);
}

template<typename T>
T* FindInterface(const char* moduleName, const char* versionString)
{
	auto CreateInterface = EXTERNAL_PROC(moduleName, "CreateInterface", T*, (const char* versionString, int* returnCode));
	if (!CreateInterface) return nullptr;

	return CreateInterface(versionString, nullptr);
}

void CS2::SetupInterfaces()
{
	InputSystem = FindInterface<IInputSystem>("inputsystem", INPUT_SYSTEM_VERSION);
	EngineClient = FindInterface<IEngineClient>("engine2", ENGINE_CLIENT_VERSION);
}
