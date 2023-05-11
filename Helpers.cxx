#include "Helpers.hxx"

#include <Windows.h>
#include <stdio.h>

#include "Globals.hxx"

void NEPS::Unload()
{
	CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)FreeLibrary, g_hModule, 0, nullptr)); // Self-destruction from a different thread :3
}

void NEPS::ErrorMsgBox(const char* titleFmt, const char* contentFmt, ...)
{
	char content[1024];
    va_list args;
    va_start(args, contentFmt);
    vsprintf_s(content, contentFmt, args);
    va_end(args);

	char title[256];
	sprintf_s(title, "NEPS2 | %s", titleFmt);

    MessageBoxA(nullptr, content, title, MB_ICONERROR | MB_OK);
}
