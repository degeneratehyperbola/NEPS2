#pragma once

#include <Windows.h>
#include <d3d11.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

#include "Globals.hxx"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

namespace Callbacks
{
	// Called each render frame
	HRESULT WINAPI Present(IDXGISwapChain* pSwapChain, UINT syncInterval, UINT flags)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		return g_hkPresent.CallOriginal<HRESULT>(pSwapChain, syncInterval, flags);
	}

	// Called when the game's window is resized
	HRESULT WINAPI ResizeBuffers(IDXGISwapChain* pSwapChain, UINT bufferCount, UINT w, UINT h, DXGI_FORMAT newFormat, UINT flags)
	{
		ImGui_ImplDX11_InvalidateDeviceObjects(); // Device objects are recreated at the next ImGui_ImplDX11_NewFrame call

		return g_hkResizeBuffers.CallOriginal<HRESULT>(pSwapChain, bufferCount, w, h, newFormat, flags);
	}

	// Calls are event-based (e.g. keypress, clipboard action)
	LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

		if (uMsg == WM_KEYDOWN && wParam == VK_END)
			CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)FreeLibrary, g_hModule, 0, nullptr)); // Self-destruction from a different thread :3

		return CallWindowProcW(g_pOriginalWndProc, hWnd, uMsg, wParam, lParam);
	}
}
