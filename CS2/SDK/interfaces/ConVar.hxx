#pragma once

#include <PCH.h>

namespace CS2
{
	struct ConVar
	{
		PROPERTY(0, const char*, Name);
		PROPERTY(64, int, GetInt);
		PROPERTY(64, float, GetFloat);
	};

	class IConVarIndex
	{
	public:
		VIRTUAL_METHOD(36, ConVar*, GetVar, (size_t index), (this, index));
	};
}
