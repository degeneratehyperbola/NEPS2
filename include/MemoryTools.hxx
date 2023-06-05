#pragma once

#include <functional> // For `reference_wrapper` and `std::ref`
#include <intrin.h> // For useful intrinsics like `_ReturnAddress`
#include <Windows.h>
#include <cstdint>

#define _STRINGIFYX(x) #x
#define _STRINGIFY(x) _STRINGIFYX(x)

#define _CONCATX(x, y) x##y
#define _CONCAT(x, y) _CONCATX(x, y)

#define PAD(size) \
private: \
    byte _CONCAT(_pad, __COUNTER__)[size]; \
public:

#define EXTERNAL_PROC(moduleName, procName, returnType, params) \
(reinterpret_cast<returnType (WINAPI*)params>(GetProcAddress(GetModuleHandleA(moduleName), procName)))

#define GET_VTABLE(obj) (*reinterpret_cast<void***>(obj))

#define VIRTUAL_METHOD(idx, returnType, name, params, passedArgs) \
inline returnType name params \
{ \
	return VirtualMethod::Call<idx, returnType>passedArgs; \
}

#define PROPERTY(type, name, offset) \
inline type name() { return *reinterpret_cast<type*>((uintptr_t)this + offset); }

#define PROPERTY_REF(type, name, offset) \
inline type& name() { return *reinterpret_cast<type*>((uintptr_t)this + offset); }

using byte = uint8_t;

namespace VirtualMethod
{
	template <size_t Idx, typename Ret, typename... Args>
	constexpr Ret Call(void* classBase, Args... args)
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

	bool IsInBounds(HMODULE hModule, uintptr_t address);
	bool IsInBounds(const char* moduleName, uintptr_t address);
	uintptr_t FindPattern(uintptr_t base, ptrdiff_t size, const char* pattern);
	uintptr_t FindPattern(const char* moduleName, const char* pattern);
}
