#include "RenderTexture.h"

RenderTexture::RenderTexture()
{
	m_srv = nullptr;
	m_rtv = nullptr;
	m_texture = nullptr;
}

RenderTexture::~RenderTexture()
{
}

bool RenderTexture::Initialize(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format, int arraySize, int mipLevels) {
	this->width = width;
	this->height = height;
	this->ammountOfViews = arraySize;

	D3D11_TEXTURE2D_DESC textureDesc;
	ZMEM(textureDesc);
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.Format = format;
	textureDesc.MipLevels = mipLevels;
	textureDesc.ArraySize = arraySize;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;

	HRESULT hResult;
	hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	HREIF;

	m_rtv = new ID3D11RenderTargetView*[arraySize];
	for (int i = 0; i < arraySize; i++) 
		m_rtv[i] = nullptr;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZMEM(rtvDesc);
	rtvDesc.Format = format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	rtvDesc.Texture2DArray.FirstArraySlice = 0;
	rtvDesc.Texture2DArray.ArraySize = arraySize;
	for (int i = 0; i < arraySize; i++) {
		rtvDesc.Texture2DArray.MipSlice = D3D11CalcSubresource(0, i, mipLevels);
		hResult = device->CreateRenderTargetView(m_texture, &rtvDesc, &m_rtv[i]);
		HREIF;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZMEM(srvDesc);
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.ArraySize = arraySize;
	srvDesc.Texture2DArray.MipLevels = mipLevels;
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.MostDetailedMip = 0;

	hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_srv);
	HREIF;

	return true;
}
void RenderTexture::Shutdown() {
	ReleaseCOM(m_texture);
	ReleaseCOM(m_srv);
	if (m_rtv)
		for (int i = 0; i < ammountOfViews; i++)
			if (m_rtv[i])
				m_rtv[i]->Release();
}