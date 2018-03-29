#include "Font.h"



Font::Font()
{
	texture = nullptr;
}


Font::~Font()
{
}

bool Font::Initialize(ID3D11Device* device) {
	texture = new Texture2D();
	std::vector<std::string> paths;
	paths.push_back("Impact");
	IFFAL(texture->Initialize(device, paths));//Load font texture array for display
	paths.clear();
	paths.push_back("Impact.fnt");
	ammountOfFonts = paths.size();

	charData = new CharData*[ammountOfFonts];
	fontData = new FontData*[ammountOfFonts];

	std::ifstream file;
	for (int i = 0; i < ammountOfFonts; i++) {
		charData[i] = new CharData[191];
		fontData[i] = new FontData[191];

		file.open(paths[i], std::ios::binary | std::ios::ate);
		IFFAL(file.good());
		int g = (int)file.tellg();
		file.seekg(std::ios::beg);
		IFFAL(g == sizeof(CharData) * 191);
		file.read((char*)&charData[i][0], g);
		file.close();
		file.clear();

		for (int j = 0; j < 191; j++) {
			fontData[i][j].u0 = (float) charData[i][j].x							/ 256.0f;
			fontData[i][j].u1 = (float)(charData[i][j].x + charData[i][j].width)	/ 256.0f;
			fontData[i][j].v0 = (float) charData[i][j].y							/ 256.0f;
			fontData[i][j].v1 = (float)(charData[i][j].y + charData[i][j].height)	/ 256.0f;
		}
	}
}
void Font::Shutdown() {
	SHUTDOWN(texture);
}
void Font::RecalculateFontData(float screenWidth, float screenHeight) {
	for (int i = 0; i < ammountOfFonts; i++) {
		for (int j = 0; j < 191; j++) {
			fontData[i][j].width = (float)charData[i][j].xadvance / screenWidth;
			fontData[i][j].height = (float)charData[i][j].height / screenHeight;
			char charRepresentation = (char)charData[i][j].id;
			if (charRepresentation == 'y' || charRepresentation == 'j' || charRepresentation == 'g' || charRepresentation == 'p' || charRepresentation == 'q') {
				fontData[i][j].yOffset = -2.0f / screenHeight;
			} else {
				fontData[i][j].yOffset = 0;
			}
			//fontData[i][j].yOffset = 0;// (float)charData[i][j].yoffset / screenHeight;
		}
	}
}