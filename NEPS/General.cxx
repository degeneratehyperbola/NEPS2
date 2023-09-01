#include "General.hxx"

#include <PCH.hpp>

#include "CS2/General.hxx"

#include "Globals.hxx"
#include "GUI.hxx"


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
int NEPS::Error(const char* msgBoxTitle, const char* msgBoxContentFmt, ...)
{
	char content[1024];
	va_list args;
	va_start(args, msgBoxContentFmt);
	vsprintf_s(content, msgBoxContentFmt, args);
	va_end(args);

	char title[256];
	sprintf_s(title, "NEPS2 | %s", msgBoxTitle);

	MessageBoxA(nullptr, content, title, MB_ICONERROR | MB_OK);
	Unload();

	return 0; // It returns FALSE just for syntactic sugar
}
