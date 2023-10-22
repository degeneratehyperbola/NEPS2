#pragma once

#include <PCH.hpp>


namespace CS2
{
	class IEngineClient
	{
	public:
		VIRTUAL_METHOD(32, bool, IsInGame, (), (this));
		VIRTUAL_METHOD(48, void, GetScreenSize, (int& width, int& height), (this, std::ref(width), std::ref(height)));
	};
}
