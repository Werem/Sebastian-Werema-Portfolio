#include "Shader.h"

Shader::Shader()
{
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_inputLayout = nullptr;
}


Shader::~Shader()
{
}

bool Shader::Initialize(ID3D11Device* device, LPCWSTR vsFilePath, LPCWSTR psFilePath, D3D11_INPUT_ELEMENT_DESC* inputDesc, int inputDescElements) {
	HRESULT hResult;
	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexBytecode = nullptr;
	ID3D10Blob* pixelBytecode = nullptr;

	hResult = D3DCompileFromFile(vsFilePath, NULL, NULL, "VSMain", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, NULL, &vertexBytecode, &errorMessage);
	if (FAILED(hResult)) {
		if (errorMessage)
			OutputShaderMessage(errorMessage, vsFilePath);
		else
			SIMPLE_MESSAGEBOX_ERROR(vsFilePath);
		ReleaseCOM(errorMessage);
		ReleaseCOM(vertexBytecode);
		ReleaseCOM(pixelBytecode);
		return false;
	}
	hResult = D3DCompileFromFile(psFilePath, NULL, NULL, "PSMain", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, NULL, &pixelBytecode, &errorMessage);
	if (FAILED(hResult)) {
		if (errorMessage)
			OutputShaderMessage(errorMessage, psFilePath);
		else
			SIMPLE_MESSAGEBOX_ERROR(psFilePath);
		ReleaseCOM(errorMessage);
		ReleaseCOM(vertexBytecode);
		ReleaseCOM(pixelBytecode);
		return false;
	}

	hResult = device->CreateVertexShader(vertexBytecode->GetBufferPointer(), vertexBytecode->GetBufferSize(), NULL, &m_vertexShader);
	HREIF;
	hResult = device->CreatePixelShader(pixelBytecode->GetBufferPointer(), pixelBytecode->GetBufferSize(), NULL, &m_pixelShader);
	HREIF;

	hResult = device->CreateInputLayout(inputDesc, inputDescElements, vertexBytecode->GetBufferPointer(), vertexBytecode->GetBufferSize(), &m_inputLayout);
	HREIF;

	return true;
}
void Shader::Shutdown() {
	ReleaseCOM(m_inputLayout);
	ReleaseCOM(m_vertexShader);
	ReleaseCOM(m_pixelShader);
}
void Shader::OutputShaderMessage(ID3D10Blob* errorBlob, LPCWSTR path) {
	char* compileError = (char*)(errorBlob->GetBufferPointer());
	unsigned long bufferSize = (unsigned long)errorBlob->GetBufferSize();
	std::ofstream fout;
	fout.open("shader-error.txt");
	for (unsigned long i = 0; i < bufferSize; i++) {
		fout << compileError[i];
	}
	fout.close();
	MessageBox(NULL, L"ERROR COMPILING SHADER", NULL, MB_OK);
}