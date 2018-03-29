#include "MainGame.h"



MainGame::MainGame()
{
}


MainGame::~MainGame()
{
}

bool MainGame::Initialize(HINSTANCE hInstance) {
	//m_dx11 = new DirectX11();
	//IFFALMESSAGE(m_dx11->Initialize(L"My Window"), L"Could Not Start DirectX11");

	m_graphics = new Graphics();
	IFFALMESSAGE(m_graphics->Initialize(hInstance), L"Could not start");
	
	return true;
}
void MainGame::Shutdown() {
	//SHUTDOWN(m_dx11);
	SHUTDOWN(m_graphics);
}
void MainGame::Run() {
	//while (!m_dx11->IsClosing()) {
	//}
	while (m_graphics->Render()) {

	}
}