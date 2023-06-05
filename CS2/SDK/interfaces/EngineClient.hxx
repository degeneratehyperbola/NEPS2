#pragma once

#include <PCH.h>

namespace CS2
{
	class IEngineClient
	{
	public:
		VIRTUAL_METHOD(30, bool, IsInGame, (), (this));
		VIRTUAL_METHOD(31, bool, IsConnected, (), (this));
		VIRTUAL_METHOD(48, void, GetScreenSize, (int& width, int& height), (this, std::ref(width), std::ref(height)));
		VIRTUAL_METHOD(51, const char*, GetLevelName, (), (this));
		VIRTUAL_METHOD(52, const char*, GetLevelNameShort, (), (this));
	};
}
