#include "MemoryTools.hxx"

#include <Windows.h>
#include <Psapi.h>
#include <vector>
#include <tuple>

#include "Helpers.hxx"

std::tuple<std::vector<byte>, std::vector<byte>> PatternToBytesAndMask(const char* pattern)
{
	std::vector<byte> bytes, mask;
	for (const char* cursor = pattern; *cursor; ++cursor)
	{
		if (*cursor == ' ')
			continue;

		const auto b = (byte)std::strtoul(cursor, const_cast<char**>(&cursor), 16);
		bytes.push_back(b);
		mask.push_back(b ? 0xFF : 0);
	}

	return {bytes, mask};
}

uintptr_t MemorySearch::FindPattern(const char* moduleName, const char* pattern, bool unloadWhenNotFound)
{
	const auto hModule = GetModuleHandleA(moduleName);
	if (!hModule)
	{
		if (unloadWhenNotFound)
		{
			NEPS::ErrorMsgBox("Module is not loaded!", "%s's handle was not retrieved.", moduleName);
			NEPS::Unload();
		}

		return 0;
	}

	MODULEINFO moduleInfo;
	if (!GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(moduleInfo)))
		return 0;

	const auto [patternBytes, patternMask] = PatternToBytesAndMask(pattern);
	const auto moduleBase = (byte*)moduleInfo.lpBaseOfDll;
	const auto moduleSize = (ptrdiff_t)moduleInfo.SizeOfImage;
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

	if (unloadWhenNotFound)
	{
		NEPS::ErrorMsgBox("Pattern not found!", "%s | %s", moduleName, pattern);
		NEPS::Unload();
	}

	return 0;
}
