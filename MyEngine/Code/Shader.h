#ifndef SHADER_H
#define SHADER_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include "PreprocessorMacros.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

class Shader
{
public:
	Shader();
	~Shader();

	bool Initialize(ID3D11Device* device, LPCWSTR vsFilePath, LPCWSTR psFilePath, D3D11_INPUT_ELEMENT_DESC* inputDesc, int inputDescElements);
	void Shutdown();

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_inputLayout;

private:

	void OutputShaderMessage(ID3D10Blob* errorBlob, LPCWSTR path);
};

#endif