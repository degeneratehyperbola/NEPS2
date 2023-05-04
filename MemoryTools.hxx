#pragma once

#include <Windows.h>
#include <Psapi.h>
#include <vector>

#define PAD(size) \
private: \
    byte _CONCAT(_pad, __COUNTER__)[size]; \
public:

#define GET_VTABLE(obj) (*reinterpret_cast<void***>(obj))

namespace MemorySearch
{
	template<typename T>
	constexpr T RelativeToAbsolute(uintptr_t address)
	{
		return reinterpret_cast<T>(address + *reinterpret_cast<int32_t*>(address) + 4);
	}

	std::vector<byte> PatternToBytes(const char* pattern)
	{
		std::vector<byte> bytes;
		for (const char* cursor = pattern; *cursor; ++cursor)
		{
			if (*cursor == ' ')
				continue;

			bytes.push_back((byte)std::strtoul(cursor, const_cast<char**>(&cursor), 16));
		}

		return bytes;
	}

	std::vector<byte> PatternToMask(const char* pattern)
	{
		std::vector<byte> bytes = PatternToBytes(pattern);
		for (auto& b : bytes)
			b = b ? 0xFF : 0;

		return bytes;
	}

	uintptr_t FindPattern(const char* moduleName, const char* pattern, bool reportNotFound = true)
	{
		const auto hModule = GetModuleHandleA(moduleName);
		if (!hModule) return 0;

		MODULEINFO moduleInfo;
		if (!GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(moduleInfo)))
			return 0;

		const auto patternBytes = PatternToBytes(pattern);
		const auto patternMask = PatternToMask(pattern);
		const auto moduleBase = (byte*)moduleInfo.lpBaseOfDll;
		const auto moduleSize = (size_t)moduleInfo.SizeOfImage;
		const auto end = moduleBase + moduleSize - patternBytes.size();
		for (byte* currentByte = moduleBase; currentByte < end; ++currentByte)
		{
			bool match = true;
			for (size_t i = 0; i < patternBytes.size(); i++)
			{
				if ((patternMask[i] & currentByte[i]) != patternBytes[i])
				{
					match = false;
					break;
				}
			}

			if (match)
				return reinterpret_cast<uintptr_t>(currentByte);
		}

		if (reportNotFound)
		{
			const size_t sz = strlen(pattern) + strlen(moduleName) + 100;
			char* text = new char[sz];
			sprintf_s(text, sz, "%s at %p :: %s", moduleName, moduleBase, pattern);
			MessageBoxA(g_hWnd, pattern, "Pattern not found!", MB_ICONERROR | MB_OK);
			delete[] text;
		}

		return 0;
	}
}
