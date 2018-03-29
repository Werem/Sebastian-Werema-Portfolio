#ifndef FONT_H
#define FONT_H

#include <d3d11.h>
#include <fstream>
#include <string>
#include <vector>
#include <DirectXMath.h>
#include "Texture2D.h"
#include "PreprocessorMacros.h"

#pragma comment (lib, "d3d11.lib")

using namespace DirectX;

class Font
{
	struct CharData {
		int id;
		int x;
		int y;
		int width;
		int height;
		int xoffset;
		int yoffset;
		int xadvance;
	};
	struct FontData {
		float u0;
		float u1;
		float v0;
		float v1;
		float width;
		float height;
		float yOffset;
	};
public:
	Font();
	~Font();

	bool Initialize(ID3D11Device* device);
	void Shutdown();
	void RecalculateFontData(float screenWidth, float screenHeight);

	//struct Text {
	//	Mesh* mesh;
	//	Vertex* vertices;
	//	char* text;
	//	int ammountOfChars;
	//	int maxAmmountOfChars;
	//	float x;
	//	float y;
	//	bool dynamic;
	//};

	Texture2D* texture;
	FontData** fontData;
private:
	int ammountOfFonts;
	CharData** charData;
};

#endif