#ifndef CONSTANT_BUFFER_H
#define CONSTANT_BUFFER_H

#include <d3d11.h>
#include "PreprocessorMacros.h"

#pragma comment (lib, "d3d11.lib")

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	bool Initialize(ID3D11Device* device, void* data, UINT dataSize, D3D11_USAGE usage, ID3D11DeviceContext* context);
	void Shutdown();
	bool Update(void* newData);

private:
	DXGI_USAGE m_usage;
	void* m_data;
	UINT m_dataSize;
	ID3D11DeviceContext* deviceContext;

	ID3D11Buffer* bufferData;
	
};

#endif