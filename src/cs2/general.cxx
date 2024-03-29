#include "general.hxx"


// Enables or disables relative mouse mode, where the mouse stays in one place but sends move events
void CS2::SetRelativeMouseMode(bool enabled)
{
	static auto fn = EXTERNAL_PROC("SDL3", "SDL_SetRelativeMouseMode", int, (bool));
	if (fn) fn(enabled);
}

// Enables or disables grabbing of mouse events
void CS2::SetMouseCapture(bool enabled)
{
	static auto fn = EXTERNAL_PROC("SDL3", "SDL_SetWindowMouseGrab", void, (void*, bool));
	if (fn) fn(InputSystem->GetSDLWindow(), enabled);
}

// Sets mouse position inside the client area
void CS2::WarpMouseInWindow(int x, int y)
{
	static auto fn = EXTERNAL_PROC("SDL3", "SDL_WarpMouseInWindow", void, (void*, int, int));
	if (fn) fn(InputSystem->GetSDLWindow(), x, y);
}

// Gets width and height of the client area
void CS2::GetWindowSize(int* w, int* h)
{
	static auto fn = EXTERNAL_PROC("SDL3", "SDL_GetWindowSize", int, (void*, int*, int*));
	if (fn) fn(InputSystem->GetSDLWindow(), w, h);
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
	InputSystem = FindInterface<IInputSystem>("inputsystem", "InputSystemVersion001");
	EngineClient = FindInterface<IEngineClient>("engine2", "Source2EngineToClient001");
}
