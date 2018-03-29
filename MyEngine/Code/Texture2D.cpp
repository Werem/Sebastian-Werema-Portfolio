#include "Texture2D.h"



Texture2D::Texture2D()
{
	shaderResourceView = nullptr;
	textureCOM = nullptr;
}


Texture2D::~Texture2D()
{
}

bool Texture2D::Initialize(ID3D11Device* device, std::vector<std::string> &paths) {
	int ammountOfTextures = paths.size();

	textureBuffers = new BYTE*[ammountOfTextures];

	TextureInfo baseInfo;
	TextureInfo currentInfo;
	std::string currentInfoName = paths[0] + ".tinf";
	std::string currentTextureName;

	std::ifstream file;
	file.open(currentInfoName, std::ios::binary);
	IFFAL(file.good());
	file.read((char*)&baseInfo, sizeof(TextureInfo));
	file.close();
	file.clear();

	std::vector<int> mipSizes;
	int currentSize = baseInfo.width;
	for (int i = 0; i < baseInfo.mipLevels; i++) {
		currentSize /= 2;
		mipSizes.push_back(currentSize);
	}

	D3D11_TEXTURE2D_DESC texDesc;
	ZMEM(texDesc);
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Width = baseInfo.width;
	texDesc.Height = baseInfo.height;
	texDesc.MipLevels = baseInfo.mipLevels == 0 ? 1 : baseInfo.mipLevels + 1;
	texDesc.CPUAccessFlags = 0;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.MiscFlags = 0;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.ArraySize = ammountOfTextures;

	D3D11_SUBRESOURCE_DATA* subData = new D3D11_SUBRESOURCE_DATA[ammountOfTextures * (baseInfo.mipLevels + 1)];

	for (int i = 0; i < ammountOfTextures; i++) {
		currentInfoName = paths[i] + ".tinf";
		currentTextureName = paths[i] + ".tex";

		file.open(currentInfoName, std::ios::binary);
		IFFALMESSAGE(file.good(), currentInfoName + L" is missing");
		file.read((char*)&currentInfo, sizeof(TextureInfo));
		file.close();
		file.clear();

		IFFALMESSAGE(currentInfo.width == baseInfo.width && currentInfo.height == baseInfo.height,
			paths[i] + L" might be corrupted and cannot be loaded");

		file.open(currentTextureName, std::ios::binary);
		IFFALMESSAGE(file.good(), currentTextureName + L" is missing");
		textureBuffers[i] = new BYTE[currentInfo.size];
		file.read((char*)textureBuffers[i], currentInfo.size);
		file.close();
		file.clear();

		int indexOfTexture = i * (baseInfo.mipLevels + 1);
		UINT currentFileLenght = baseInfo.width * baseInfo.height * 4 * sizeof(BYTE);
		subData[indexOfTexture].pSysMem = textureBuffers[i];
		subData[indexOfTexture].SysMemPitch = baseInfo.width * 4 * sizeof(BYTE);
		subData[indexOfTexture].SysMemSlicePitch = baseInfo.width * baseInfo.height * 4 * sizeof(BYTE);
		for (int j = 0; j < baseInfo.mipLevels; j++) {
			int mipTextureIndex = indexOfTexture + j + 1;
			subData[mipTextureIndex].pSysMem = &textureBuffers[i][currentFileLenght];
			subData[mipTextureIndex].SysMemPitch = mipSizes[j] * 4 * sizeof(BYTE);
			subData[mipTextureIndex].SysMemSlicePitch = mipSizes[j] * mipSizes[j] * 4 * sizeof(BYTE);
			currentFileLenght += subData[mipTextureIndex].SysMemSlicePitch;
		}

	}
	HRESULT hResult = device->CreateTexture2D(&texDesc, subData, &textureCOM);
	HREIF;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZMEM(srvDesc);
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.ArraySize = ammountOfTextures;
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	hResult = device->CreateShaderResourceView(textureCOM, &srvDesc, &shaderResourceView);
	HREIF;
	latestIndex++;
	return true;
}
void Texture2D::Shutdown() {
	ReleaseCOM(textureCOM);
	ReleaseCOM(shaderResourceView);
}