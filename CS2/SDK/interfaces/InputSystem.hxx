#pragma once

#include <PCH.hpp>


namespace CS2
{
	class IInputSystem
	{
	public:
		PROPERTY(79, bool, WantToCaptureMouse);
		PROPERTY(9840, void*, GetSDLWindow);
	};
}
