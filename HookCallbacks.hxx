#pragma once

#include <Windows.h>
#include <d3d11.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

#include "Globals.hxx"

namespace Callbacks
{
	HRESULT WINAPI Present(IDXGISwapChain* pSwapChain, UINT syncInterval, UINT flags)
	{
		ImGui_ImplDX11_NewFrame();

		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		return g_hmPresent.CallOriginal<HRESULT>(pSwapChain, syncInterval, flags);
	}

	HRESULT WINAPI ResizeBuffers(IDXGISwapChain* pSwapChain, UINT bufferCount, UINT w, UINT h, DXGI_FORMAT newFormat, UINT flags)
	{
		ImGui_ImplDX11_InvalidateDeviceObjects();

		return g_hmResizeBuffers.CallOriginal<HRESULT>(pSwapChain, bufferCount, w, h, newFormat, flags);
	}

	LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

		if (uMsg == WM_KEYDOWN && wParam == VK_END)
			FreeLibrary(g_hModule);

		return CallWindowProcW(g_pOriginalWndProc, hWnd, uMsg, wParam, lParam);
	}
}
