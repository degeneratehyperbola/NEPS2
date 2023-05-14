#pragma once

#include <cstdint>

#define PAD(size) \
private: \
    byte _CONCAT(_pad, __COUNTER__)[size]; \
public:

#define GET_VTABLE(obj) (*reinterpret_cast<void***>(obj))

#define VIRTUAL_METHOD(returnType, name, idx, params, passedArgs) \
returnType name params \
{ \
	return VirtualMethod::call<idx, returnType>passedArgs; \
}

using byte = uint8_t;

namespace VirtualMethod
{
	template <size_t Idx, typename Ret, typename... Args>
	constexpr Ret call(void* classBase, Args... args)
	{
		void** vmt = GET_VTABLE(classBase);
		auto virtualMethod = reinterpret_cast<Ret(__thiscall*)(void*, Args...)>(vmt[Idx]);
		return virtualMethod(classBase, args...);
	}
}

namespace MemorySearch
{
	template<typename T>
	constexpr T RelativeToAbsolute(uintptr_t address)
	{
		return reinterpret_cast<T>(address + *reinterpret_cast<int32_t*>(address) + 4);
	}

	uintptr_t FindPattern(const byte* base, ptrdiff_t size, const char* pattern);
	uintptr_t FindPatternInModule(const char* moduleName, const char* pattern);
}
