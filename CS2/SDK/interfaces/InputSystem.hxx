#pragma once

#include <PCH.h>

namespace CS2
{
	class IInputSystem
	{
	public:
		PROPERTY(bool, IsRelativeMouseMode, 79);
		PROPERTY(void*, GetSDLWindow, 9840);
	};
}
