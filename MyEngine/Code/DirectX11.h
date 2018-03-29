#ifndef DIRECTX_H
#define DIRECTX_H

#include <d3d11.h>
#include <fstream>
#include <vector>
#include "PreprocessorMacros.h"

#pragma comment (lib, "d3d11.lib")

static const float NEAR_Z = 0.001f;
static const float FAR_Z = 1000.0f;

struct Resolution {
	int width;
	int height;
	byte left;
	byte right;
	Resolution(int width, int height, byte left, byte right) : width(width), height(height), left(left), right(right) {}
};

static const Resolution SCREEN_RESOLUTIONS[] = {
	Resolution(640 , 480, 4 , 3),
	Resolution(800 , 480, 5 , 3),
	Resolution(1024, 600, 16, 9),
	Resolution(1024, 768, 4 , 3),
	Resolution(1200, 900, 4 , 3),
	Resolution(1280,1024, 5 , 4),
	Resolution(1440,900	, 16, 10),
	Resolution(1680,1050, 16, 10),
	Resolution(1600,900	, 16, 9),
	Resolution(1600,1200, 4 , 3),
	Resolution(1366,768	, 16, 9),
	Resolution(1368,768	, 16, 9),
	Resolution(1920,1200, 16, 10),
	Resolution(2560,1600, 16, 10),
	Resolution(1280,720	, 16, 9),
	Resolution(1920,1080, 16, 9),
	Resolution(2560,1440, 16, 9),
	Resolution(2560,1600, 16, 10),
	Resolution(3840,2160, 16, 9)
}; //19

class DirectX11
{
public:
	struct Settings {
		UINT screenWidth;
		UINT screenHeight;
		int MSAAType;
		bool vSync;
		bool customResolution;
		float brightness;
	};

	DirectX11();
	~DirectX11();

	bool CreateDirectX11Window(LPCWSTR windowName, bool isFullscreen);
	bool Initialize(LPCWSTR windowName);
	bool IsClosing();
	void Shutdown();
	bool ChangeSettings(Settings &newSettings);
	void ClearScreen();
	void PresentScene();

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_context;
private:
	HWND m_hwnd;
	D3D_FEATURE_LEVEL m_featureLevel;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RenderTargetView* m_renderTargetView;
	IDXGISwapChain* m_swapChain;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11Texture2D* m_swapChainBuffer;
	IDXGIFactory* m_dxgiFactory;

	Settings settings;
	UINT supported2xMSAALevels;
	UINT supported4xMSAALevels;

	IDXGIAdapter** videoAdapters;
	int ammountOfAdapters = 0;

	bool AlreadyInitialized = false;

	bool ApplySettings();
	bool CreateGameWindow();
	bool CreateDirectX();
	bool EnumerateAdapters();
	void SetDefaultSettings();
	int LookForClosestResolution();
	int LookForBestAdapter();

	MSG m_msg;
	LPCWSTR m_windowName;

};
static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
	case WM_QUIT:
	case WM_CLOSE: {
		MessageBox(NULL, L"Closing", NULL, MB_OK);
		PostQuitMessage(0);
		return 0;
	}
	case WM_KEYDOWN: {
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
			return 0;
		}
	}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
#endif