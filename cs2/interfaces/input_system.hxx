#pragma once

#include <pch.hpp>


namespace CS2
{
	class IInputSystem
	{
	public:
		PROPERTY(79, bool, WantToCaptureMouse);
		PROPERTY(9840, void*, GetSDLWindow);
	};
}
