#pragma once

#include <Windows.h>
#include <Psapi.h>
#include <vector>

namespace MemorySearch
{
	std::vector<byte> PatternToBytes(const char* pattern)
	{
		std::vector<byte> bytes;
		for (const char* cursor = pattern; *cursor; ++cursor)
		{
			if (*cursor == ' ')
				continue;

			const auto cursorByte = static_cast<byte>(std::strtoul(cursor, const_cast<char**>(&cursor), 16));
			bytes.push_back(cursorByte);
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

	void* FindPattern(const char* moduleName, const char* pattern, bool reportNotFound = true)
	{
		const auto hModule = GetModuleHandleA(moduleName);
		if (!hModule) return nullptr;

		MODULEINFO moduleInfo;
		if (!GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(moduleInfo)))
			return nullptr;

		// TODO
	}
}
