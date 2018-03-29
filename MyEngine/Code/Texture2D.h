#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <d3d11.h>
#include <string>
#include <vector>
#include <fstream>
#include <stdint.h>
#include "PreprocessorMacros.h"

#pragma comment (lib, "d3d11.lib")

static const int AMMOUNT_OF_TEXTURE_ARRAYS = 8;

class Texture2D
{
	struct TextureInfo {
		UINT width;
		UINT height;
		UINT size;
		UINT mipLevels;
	};
public:
	Texture2D();
	~Texture2D();

	bool Initialize(ID3D11Device* device, std::vector<std::string> &paths);
	void Shutdown();
	ID3D11ShaderResourceView* shaderResourceView;
private:
	ID3D11Texture2D* textureCOM;
	BYTE** textureBuffers;
	int latestIndex = 0;
};

#endif