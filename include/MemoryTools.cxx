#include "MemoryTools.hxx"

#include <Windows.h>
#include <Psapi.h>
#include <vector>
#include <tuple>

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

	return { bytes, mask };
}

uintptr_t MemorySearch::FindPattern(const byte* base, ptrdiff_t size, const char* pattern)
{
	const auto [patternBytes, patternMask] = PatternToBytesAndMask(pattern);
	const auto end = base + size - patternBytes.size();
	for (const byte* currentByte = base; currentByte < end; ++currentByte)
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

	return 0;
}

uintptr_t MemorySearch::FindPatternInModule(const char* moduleName, const char* pattern)
{
	const auto hModule = GetModuleHandleA(moduleName);
	if (!hModule)
		return 0;

	MODULEINFO moduleInfo;
	if (!GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(moduleInfo)))
		return 0;

	const auto moduleBase = (byte*)moduleInfo.lpBaseOfDll;
	const auto moduleSize = (ptrdiff_t)moduleInfo.SizeOfImage;
	
	return FindPattern(moduleBase, moduleSize, pattern);
}
