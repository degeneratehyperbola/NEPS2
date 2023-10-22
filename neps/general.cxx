#include "general.hxx"

#include "cs2/general.hxx"

#include "globals.hxx"
#include "gui.hxx"


// Self-destruction from a different thread :3
void NEPS::Unload()
{
	// Restore mouse polling
	if (CS2::InputSystem->WantToCaptureMouse())
	{
		CS2::SetRelativeMouseMode(true);
		CS2::SetMouseCapture(true);
	}

	CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)FreeLibrary, g_hModule, 0, nullptr));
}

// Show a message box and unload
bool NEPS::FatalError(const char* msgBoxTitle, const char* msgBoxContentFmt, ...)
{
	char content[1024];
	va_list args;
	va_start(args, msgBoxContentFmt);
	vsprintf_s(content, msgBoxContentFmt, args);
	va_end(args);

	char title[256];
	sprintf_s(title, "NEPS2 Fatal Error | %s", msgBoxTitle);

	MessageBoxA(nullptr, content, title, MB_ICONERROR | MB_OK);
	Unload();

	return false; // It returns false so we can just return the result of this function in a fatal error situation. Just for syntactic sugar <3
}
