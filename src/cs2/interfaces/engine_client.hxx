#pragma once

#include <pch.hpp>


namespace CS2
{
	class IEngineClient
	{
	public:
		VIRTUAL_METHOD(32, bool, IsInGame, (), (this));
	};
}
