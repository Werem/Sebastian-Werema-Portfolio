#include "Text.h"



Text::Text()
{
	mesh = nullptr;
}


Text::~Text()
{
}

bool Text::Initialize(ID3D11Device* device, Font* font, float x, float y, float yOffsetScale, float textScale, std::string &textToRender, int additionalSlotsToCreate, D3D11_USAGE usage) {
	int ammountOfChars = textToRender.length();
	this->yOffsetScale = yOffsetScale;
	this->x = x;
	this->y = y;
	this->maxAmmountOfChars = ammountOfChars + additionalSlotsToCreate;
	this->usage = usage;

	string = new char[maxAmmountOfChars];
	for (int i = 0; i < ammountOfChars; i++) {
		string[i] = textToRender.at(i);
	}

	vertices = new Vertex[maxAmmountOfChars * 4];
	indices = new UINT16[maxAmmountOfChars * 6];
	int charID = 0;
	float xPos = x;

	for (int i = 0; i < ammountOfChars; i++) {

		charID = (int)string[i];
		if (charID <= 126 && charID >= 32) {
			charID -= 32;
		} else if (charID >= 160 && charID <= 255) {
			charID -= 65;
		} else {
			DEBUG_LINE_AND_FILE;
		}

		vertices[i * 4].position.x = xPos;
		vertices[i * 4].position.y = y + font->fontData[0][charID].yOffset * yOffsetScale;
		vertices[i * 4].position.z = 0.5f;
		vertices[i * 4].uv.x = font->fontData[0][charID].u0;
		vertices[i * 4].uv.y = font->fontData[0][charID].v1;
		vertices[i * 4].uv.z = 0.0f;

		vertices[i * 4 + 1].position.x = xPos;
		vertices[i * 4 + 1].position.y = y + font->fontData[0][charID].yOffset * yOffsetScale + font->fontData[0][charID].height * textScale;
		vertices[i * 4 + 1].position.z = 0.5f;
		vertices[i * 4 + 1].uv.x = font->fontData[0][charID].u0;
		vertices[i * 4 + 1].uv.y = font->fontData[0][charID].v0;
		vertices[i * 4 + 1].uv.z = 0.0f;

		vertices[i * 4 + 2].position.x = xPos + font->fontData[0][charID].width * textScale;
		vertices[i * 4 + 2].position.y = y + font->fontData[0][charID].yOffset * yOffsetScale + font->fontData[0][charID].height * textScale;
		vertices[i * 4 + 2].position.z = 0.5f;
		vertices[i * 4 + 2].uv.x = font->fontData[0][charID].u1;
		vertices[i * 4 + 2].uv.y = font->fontData[0][charID].v0;
		vertices[i * 4 + 2].uv.z = 0.0f;

		vertices[i * 4 + 3].position.x = xPos + font->fontData[0][charID].width * textScale;
		vertices[i * 4 + 3].position.y = y + font->fontData[0][charID].yOffset * yOffsetScale;
		vertices[i * 4 + 3].position.z = 0.5f;
		vertices[i * 4 + 3].uv.x = font->fontData[0][charID].u1;
		vertices[i * 4 + 3].uv.y = font->fontData[0][charID].v1;
		vertices[i * 4 + 3].uv.z = 0.0f;

		xPos += font->fontData[0][charID].width * textScale;

		indices[i * 6] = i * 4 + 0;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
		indices[i * 6 + 3] = i * 4 + 2;
		indices[i * 6 + 4] = i * 4 + 3;
		indices[i * 6 + 5] = i * 4 + 0;
	}

	mesh = new Mesh();
	IFFAL(mesh->Initialize(device, (void*)vertices, indices, maxAmmountOfChars * 6, sizeof(Vertex), maxAmmountOfChars * 4, usage));
	indexAmmount = ammountOfChars * 6;
	return true;
}
bool Text::UpdateText(ID3D11Device* device, Font* font, float x, float y, std::string &texToRender) {
	IFFAL(usage != D3D11_USAGE_IMMUTABLE);
	return true;

}
void Text::Shutdown() {
	SHUTDOWN(mesh);
}