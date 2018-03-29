#ifndef MESH_H
#define MESH_H

#include <d3d11.h>
#include "PreprocessorMacros.h"

#pragma comment (lib, "d3d11.lib")

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool Initialize(ID3D11Device* device, void* bufferData, UINT16* indices, int indexCount, int sizeOfStruct, int ammountOfVertices, D3D11_USAGE usage);
	void Shutdown();

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int indexCount;
	UINT16* indices;
	void* bufferData;
	UINT stride;
	UINT offset = 0;
};

#endif