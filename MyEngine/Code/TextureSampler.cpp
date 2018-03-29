#include "TextureSampler.h"



TextureSampler::TextureSampler()
{
	samplerState = nullptr;
}


TextureSampler::~TextureSampler()
{
}

bool TextureSampler::Initialize(ID3D11Device* device, D3D11_TEXTURE_ADDRESS_MODE uvMode, D3D11_FILTER filter, UINT anisotropy) {
	D3D11_SAMPLER_DESC samplerDesc;
	ZMEM(samplerDesc);
	//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.Filter = filter;
	samplerDesc.AddressU = uvMode;
	samplerDesc.AddressV = uvMode;
	samplerDesc.AddressW = uvMode;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxAnisotropy = anisotropy;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MinLOD = 0;
	samplerDesc.MipLODBias = 0.0f;

	HRESULT hResult = device->CreateSamplerState(&samplerDesc, &samplerState);
	HREIF;
	return true;
}

void TextureSampler::Shutdown() {
	ReleaseCOM(samplerState);
}