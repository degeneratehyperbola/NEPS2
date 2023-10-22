#pragma once

template <typename T>
constexpr auto WinLinux(T windowsValue, T linuxValue) {
#ifdef _WIN32
	return windowsValue;
#elif __linux__
	return linuxValue;
#else
#error Unsupported platform
#endif
}
