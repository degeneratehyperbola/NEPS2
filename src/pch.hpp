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
#define IMGUI_DEFINE_MATH_OPERATORS // Until I implement Vector2 types
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_vulkan.h>
#include <imgui/imgui_impl_win32.h>

// MinHook
#include <minhook/MinHook.h>

// Custom tools
#include <memory_tools.hxx>
#include <hook_manager.hxx>
