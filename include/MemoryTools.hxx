#pragma once

#include <cstdint>

// Not exactly a "Memory Tool" but aiii vhatever :3
#define _NAMEOFX(x) #x
#define _NAMEOF(x) _NAMEOFX(x)

#define _CONCATX(x, y) x##y
#define _CONCAT(x, y) _CONCATX(x, y)

#define PAD(size) \
private: \
    byte _CONCAT(_pad, __COUNTER__)[size]; \
public:

#define GET_VTABLE(obj) (*reinterpret_cast<void***>(obj))

#define VIRTUAL_METHOD(idx, returnType, name, params, passedArgs) \
inline returnType name params \
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
