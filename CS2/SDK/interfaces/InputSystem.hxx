#pragma once

#include <MemoryTools.hxx>

#define INPUT_SYSTEM_VERSION "InputSystemVersion001"

namespace CS2
{
	class IInputSystem
	{
	public:
		PROPERTY(bool, IsRelativeMouseMode, 79);
		PROPERTY(void*, GetSDLWindow, 9840);
	};
}
