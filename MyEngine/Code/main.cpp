#include "MainGame.h"
#define WIN32_LEAN_AND_MEAN

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevHInstance, LPSTR cmdStr, int cmdInt) {
	MainGame* mainGame = new MainGame();
	if (!mainGame->Initialize(hInstance)) {
		mainGame->Shutdown();
		return 0;
	}
	mainGame->Run();
	mainGame->Shutdown();
	return 0;
}