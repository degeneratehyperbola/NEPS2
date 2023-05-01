#pragma once

#include <Windows.h>
#include <d3d11.h>

#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui.h>

#include "Globals.hxx"

HRESULT WINAPI PresentCallback(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	[[maybe_unused]] static const auto init = [](IDXGISwapChain* pSwapChain)
	{
		ID3D11Device* pDevice = nullptr;
		ID3D11DeviceContext* pDeviceContext = nullptr;

		pSwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&pDevice));
		pDevice->GetImmediateContext(&pDeviceContext);
		
		ImGui_ImplDX11_Init(pDevice, pDeviceContext);

		return true;
	}(pSwapChain);

	HRESULT hr = Hooks::g_hmPresentHook.CallOriginal<HRESULT>(pSwapChain, SyncInterval, Flags);

	ImGui_ImplDX11_NewFrame();

	ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return hr;
}
