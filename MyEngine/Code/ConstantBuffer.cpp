#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::~ConstantBuffer()
{
}

bool ConstantBuffer::Initialize(ID3D11Device* device, void* data, UINT dataSize, D3D11_USAGE usage, ID3D11DeviceContext* context) {
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth = dataSize;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.Usage = usage;

	D3D11_SUBRESOURCE_DATA subData;
	ZMEM(subData);
	subData.pSysMem = data;
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;

	device->CreateBuffer(&bufferDesc, &subData, &bufferData);

	m_usage = usage;
	m_dataSize = dataSize;
	m_data = data;
	deviceContext = context;

	return true;
}
void ConstantBuffer::Shutdown() {
	delete[] m_data;
	ReleaseCOM(bufferData);
}
bool ConstantBuffer::Update(void* newData) {
	//D3D11_MAPPED_SUBRESOURCE mapData;
	//ZMEM(mapData);
	//deviceContext->Map(bufferData, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapData);

	return true;
}