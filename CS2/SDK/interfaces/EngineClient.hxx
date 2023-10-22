#pragma once

#include <PCH.hpp>


namespace CS2
{
	class IEngineClient
	{
	public:
		VIRTUAL_METHOD(32, bool, IsInGame, (), (this));
	};
}
