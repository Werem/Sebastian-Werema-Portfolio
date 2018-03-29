#ifndef TEXT_H
#define TEXT_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include "Mesh.h"
#include "Font.h"
#include "PreprocessorMacros.h"

#pragma comment (lib, "d3d11.lib")

using namespace DirectX;

class Text
{
	struct Vertex {
		XMFLOAT3 position;
		XMFLOAT3 uv;
		void Assign(float x, float y, float u, float v, float w) {
			position.x = x;
			position.y = y;
			position.z = 0.0f;
			uv.x = u;
			uv.y = v;
			uv.z = w;
		}
	};
public:
	Text();
	~Text();

	bool Initialize(ID3D11Device* device, Font* font, float x, float y, float yOffsetScale, float textScale, std::string &textToRender, int additionalSlotsToCreate, D3D11_USAGE usage);
	bool UpdateText(ID3D11Device* device, Font* font, float x, float y, std::string &texToRender);
	void Shutdown();

	Mesh* mesh;
	int indexAmmount;
private:
	float x;
	float y;
	char* string;
	int maxAmmountOfChars;
	D3D11_USAGE usage;
	float yOffsetScale;

	Vertex* vertices;
	UINT16* indices;
};

#endif