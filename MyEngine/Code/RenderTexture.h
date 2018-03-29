#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include <d3d11.h>
#include "PreprocessorMacros.h"

#pragma comment (lib, "d3d11.lib")

class RenderTexture
{
public:
	RenderTexture();
	~RenderTexture();

	bool Initialize(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format, int arraySize, int mipLevels);
	void Shutdown();

	ID3D11ShaderResourceView* m_srv;
	ID3D11RenderTargetView** m_rtv;
private:
	ID3D11Texture2D* m_texture;
	UINT width;
	UINT height;
	int ammountOfViews;
};

#endif