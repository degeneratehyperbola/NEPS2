// Built-in
#include <stdio.h>
#include <Windows.h>

// Standard framework
#include <list>
#include <vector>
#include <array>
#include <filesystem>

namespace fs = std::filesystem;

// DirectX 11
#include <d3d11.h>

// Vulkan
#include <vulkan/vulkan.hpp>

// ImGui
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_vulkan.h>
#include <imgui/imgui_impl_win32.h>

// MinHook
#include <minhook/MinHook.h>

// Python
#include <Python.h>

// Pybind11
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

namespace py = pybind11;

// Custom tools
#include <MemoryTools.hxx>
#include <HookManager.hxx>
