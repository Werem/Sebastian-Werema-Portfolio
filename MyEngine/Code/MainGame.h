#ifndef MAIN_GAME_H
#define MAIN_GAME_H

//#include "DirectX11.h"
#include "Graphics.h"

class MainGame
{
public:
	MainGame();
	~MainGame();

	bool Initialize(HINSTANCE hInstance);
	void Shutdown();
	void Run();

private:
	//DirectX11* m_dx11;
	Graphics* m_graphics;


};

#endif