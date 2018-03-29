#ifndef GRAPHICS_H
#define GRAPHICS_H

#define TESTING_DRAWING

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include "PreprocessorMacros.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Texture2D.h"
#include "Font.h"
#include "Text.h"
#include "TextureSampler.h"
#include "RenderTexture.h"


#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3d11.lib")

class Graphics
{
public:
	Graphics();
	~Graphics();

	bool Initialize(HINSTANCE hInstance);
	void Shutdown();
	bool Render();
	
private:

	bool InitializeWindow();
	void ShutdownWindow();
	bool InitializeDirectX11();
	void ShutdownDirectx11();
	bool testInit();
	bool testDraw();
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	MSG m_msg;
	float m_screenWidth;
	float m_screenHeight;

	D3D_FEATURE_LEVEL m_featureLevel;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	IDXGISwapChain* m_swapChain;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	UINT MSAAx4Level;
	UINT MSAAx2Level;

	Texture2D* mainTexture;
	TextureSampler* wrapSampler;
	Font* fonts;
	Text* myText;
	Shader* spriteShader;
	RenderTexture* renderTexture;
#ifdef TESTING_DRAWING
	Shader* testShader;
	Mesh* testMesh;
	Camera* testCamera;
	ID3D11Buffer* testBuffer;
#endif
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif