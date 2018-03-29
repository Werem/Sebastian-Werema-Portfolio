#ifndef TEXTURE_SAMPLER_H
#define TEXTURE_SAMPLER_H

#include <d3d11.h>
#include "PreprocessorMacros.h"

#pragma comment (lib, "d3d11.lib")

class TextureSampler
{
public:
	TextureSampler();
	~TextureSampler();

	bool Initialize(ID3D11Device* device, D3D11_TEXTURE_ADDRESS_MODE uvMode, D3D11_FILTER filter, UINT anisotropy = 1);
	void Shutdown();
	ID3D11SamplerState* samplerState;
};

#endif