#include "Mesh.h"



Mesh::Mesh()
{
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
}


Mesh::~Mesh()
{
}

bool Mesh::Initialize(ID3D11Device* device, void* bufferData, UINT16* indices, int indexCount, int sizeOfStruct, int ammountOfVertices, D3D11_USAGE usage) {
	this->indexCount = indexCount;
	this->bufferData = bufferData;
	this->indices = indices;
	this->stride = sizeOfStruct;
	HRESULT hResult;

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZMEM(vertexBufferDesc);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeOfStruct * ammountOfVertices;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	vertexBufferDesc.Usage = usage;
	D3D11_SUBRESOURCE_DATA vertexSubData;
	ZMEM(vertexSubData);
	vertexSubData.pSysMem = bufferData;
	vertexSubData.SysMemPitch = 0;
	vertexSubData.SysMemSlicePitch = 0;
	hResult = device->CreateBuffer(&vertexBufferDesc, &vertexSubData, &vertexBuffer);
	if (FAILED(hResult))
		return false;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZMEM(indexBufferDesc);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = sizeof(UINT16) * indexCount;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	indexBufferDesc.Usage = usage;
	D3D11_SUBRESOURCE_DATA indexSubData;
	ZMEM(indexSubData);
	indexSubData.pSysMem = indices;
	indexSubData.SysMemPitch = 0;
	indexSubData.SysMemSlicePitch = 0;
	hResult = device->CreateBuffer(&indexBufferDesc, &indexSubData, &indexBuffer);
	if (FAILED(hResult))
		return false;

	return true;
}
void Mesh::Shutdown() {
	ReleaseCOM(vertexBuffer);
	ReleaseCOM(indexBuffer);
	delete[] indices;
	delete[] bufferData;
}