#include "MemoryTools.hxx"

#include <Windows.h>
#include <Psapi.h>
#include <vector>
#include <tuple>


std::tuple<uintptr_t, ptrdiff_t> GetBounds(HMODULE hModule)
{
	MODULEINFO moduleInfo;
	if (!GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(moduleInfo)))
		return { 0, 0 };

	return { reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll), moduleInfo.SizeOfImage };
}

bool MemorySearch::IsInBounds(HMODULE hModule, uintptr_t address)
{
	const auto [moduleBase, moduleSize] = GetBounds(hModule);
	return (address >= moduleBase) && (address < moduleBase + moduleSize);
}

bool MemorySearch::IsInBounds(const char* moduleName, uintptr_t address)
{
	const auto hModule = GetModuleHandleA(moduleName);
	if (!hModule)
		return false;

	return IsInBounds(hModule, address);
}

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

uintptr_t MemorySearch::FindPattern(const uintptr_t base, ptrdiff_t size, const char* pattern)
{
	const auto [patternBytes, patternMask] = PatternToBytesAndMask(pattern);
	const byte* end = reinterpret_cast<byte*>(base) + size - patternBytes.size();
	for (const byte* currentByte = reinterpret_cast<byte*>(base); currentByte < end; ++currentByte)
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

uintptr_t MemorySearch::FindPattern(const char* moduleName, const char* pattern)
{
	const auto hModule = GetModuleHandleA(moduleName);
	if (!hModule)
		return 0;

	const auto [moduleBase, moduleSize] = GetBounds(hModule);
	return FindPattern(moduleBase, moduleSize, pattern);
}
