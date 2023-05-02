#pragma once

#include <Windows.h>
#include <d3d11.h>

#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui.h>

#include "Globals.hxx"

namespace Callbacks
{
	HRESULT WINAPI Present(IDXGISwapChain* pSwapChain, UINT syncInterval, UINT flags)
	{
		HRESULT hr = g_hmPresent.CallOriginal<HRESULT>(pSwapChain, syncInterval, flags);

		ImGui_ImplDX11_NewFrame();

		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		return hr;
	}

	HRESULT WINAPI ResizeBuffers(IDXGISwapChain* pSwapChain, UINT bufferCount, UINT w, UINT h, DXGI_FORMAT newFormat, UINT flags)
	{
		HRESULT hr = g_hmResizeBuffers.CallOriginal<HRESULT>(pSwapChain, bufferCount, w, h, newFormat, flags);

		ImGui_ImplDX11_InvalidateDeviceObjects();

		if (SUCCEEDED(hr))
			ImGui_ImplDX11_CreateDeviceObjects();

		return hr;
	}
}
