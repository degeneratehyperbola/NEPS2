#pragma once

#include <PCH.hpp>


namespace Callbacks
{
	HRESULT WINAPI Present(IDXGISwapChain* pSwapChain, UINT syncInterval, UINT flags);
	HRESULT WINAPI ResizeBuffers(IDXGISwapChain* pSwapChain, UINT bufferCount, UINT w, UINT h, DXGI_FORMAT newFormat, UINT flags);
	LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}
