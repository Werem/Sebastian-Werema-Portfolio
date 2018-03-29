#include "DirectX11.h"



DirectX11::DirectX11()
{
	m_device = nullptr;
	m_context = nullptr;
	m_renderTargetView = nullptr;
	m_depthStencilView = nullptr;
	m_depthStencilBuffer = nullptr;
}


DirectX11::~DirectX11()
{
}

bool DirectX11::CreateDirectX11Window(LPCWSTR windowName, bool isFullscreen) {
	HRESULT hResult;
	m_windowName = windowName;
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
	#ifdef DEBUG_MODE
		createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
	#endif 

	D3D_FEATURE_LEVEL FeatureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	IFFAL(EnumerateAdapters());
	int indexOfAdapter = LookForBestAdapter();

	hResult = D3D11CreateDevice(videoAdapters[indexOfAdapter], D3D_DRIVER_TYPE_HARDWARE, NULL,
		createDeviceFlags, FeatureLevels, 6, D3D11_SDK_VERSION, &m_device, &m_featureLevel, &m_context);
	HREIFMESSAGE(L"DirectX 11 missing");


	m_dxgiFactory = 0;
	hResult = videoAdapters[indexOfAdapter]->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgiFactory);
	HREIF;
}

bool DirectX11::Initialize(LPCWSTR windowName) {
	HRESULT hResult;
	m_windowName = windowName;
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
	#ifdef DEBUG_MODE
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	D3D_FEATURE_LEVEL FeatureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	IFFAL(EnumerateAdapters());
	int indexOfAdapter = LookForBestAdapter();

	hResult = D3D11CreateDevice(videoAdapters[indexOfAdapter], D3D_DRIVER_TYPE_UNKNOWN, NULL, createDeviceFlags, FeatureLevels, 6, D3D11_SDK_VERSION, &m_device, &m_featureLevel, &m_context);
	HREIFMESSAGE(L"DirectX 11 missing!");

	std::ifstream file;
	file.open("Display.settings", std::ios::binary | std::ios::ate);
	if (file.fail()) {
		file.close();
		file.clear();
		SetDefaultSettings();
	} else {
		int fileLenght = (int)file.tellg();
		if (fileLenght != sizeof(Settings)) {
			SetDefaultSettings();
		} else {
			file.seekg(std::ios::beg);
			file.read((char*)&settings, sizeof(Settings));
		}
		file.close();
		file.clear();
	}

	m_dxgiFactory = 0;
	hResult = videoAdapters[indexOfAdapter]->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgiFactory);
	HREIF;

	return ApplySettings();
}

void DirectX11::Shutdown() {
	ChangeDisplaySettings(NULL, 0);
	m_swapChain->SetFullscreenState(false, NULL);
	ReleaseCOM(m_device);
	ReleaseCOM(m_context);
	ReleaseCOM(m_swapChain);
	ReleaseCOM(m_renderTargetView);
	ReleaseCOM(m_depthStencilView);
	ReleaseCOM(m_depthStencilBuffer);
	ReleaseCOM(m_swapChainBuffer);
	ReleaseCOM(m_dxgiFactory);
	for (int i = 0; i < ammountOfAdapters; i++) {
		ReleaseCOM(videoAdapters[i]);
	}
}

void DirectX11::SetDefaultSettings() {
	settings.vSync = true;
	int indexOfClosest = LookForClosestResolution();
	settings.screenWidth = SCREEN_RESOLUTIONS[indexOfClosest].width;
	settings.screenHeight = SCREEN_RESOLUTIONS[indexOfClosest].height;
	settings.brightness = 0.5f;
	settings.customResolution = false;
	settings.vSync = true;

	HRESULT hResult;

	hResult = m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &supported4xMSAALevels);
	if (FAILED(hResult))
		supported4xMSAALevels = 0;

	hResult = m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 2, &supported2xMSAALevels);
	if (FAILED(hResult))
		supported2xMSAALevels = 0;
	

	std::ofstream file;
	file.open("Display.settings", std::ios::binary);
	file.write((char*)&settings, sizeof(Settings));
	file.close();
	file.clear();
}
bool DirectX11::CreateGameWindow() {
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wc;
	ZMEM(wc);
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(COLOR_WINDOW);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszMenuName = NULL;
	wc.hIconSm = wc.hIcon;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = m_windowName;

	IFFAL(RegisterClassEx(&wc));

	DEVMODE dmSettings;
	ZMEM(dmSettings);

	dmSettings.dmSize = sizeof(dmSettings);
	dmSettings.dmPelsWidth = (UINT)settings.screenWidth;
	dmSettings.dmPelsHeight = (UINT)settings.screenHeight;
	dmSettings.dmBitsPerPel = 32;
	dmSettings.dmFields = DM_BITSPERPEL | DM_PELSHEIGHT | DM_PELSWIDTH;

	ChangeDisplaySettings(&dmSettings, CDS_FULLSCREEN);

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_windowName, m_windowName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, 
		0, 0, settings.screenWidth, settings.screenHeight, NULL, NULL, hInstance, NULL);
	IFEXIST(m_hwnd);

	ShowWindow(m_hwnd, SW_SHOWMAXIMIZED);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
	return true;
}
bool DirectX11::IsClosing() {
	if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE)) {
		if (m_msg.message == WM_QUIT) {
			return true;
		}
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
	}
	return false;
}
bool DirectX11::CreateDirectX() {
	HRESULT hResult;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZMEM(swapChainDesc);
	swapChainDesc.BufferDesc.Width = (UINT)settings.screenWidth;
	swapChainDesc.BufferDesc.Height = (UINT)settings.screenHeight;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	if (settings.MSAAType == 0) {
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	} else if (settings.MSAAType == 2) {
		if (supported2xMSAALevels > 0) {
			swapChainDesc.SampleDesc.Count = 2;
			swapChainDesc.SampleDesc.Quality = supported2xMSAALevels;
		} else {
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
		}
	} else {
		if (supported4xMSAALevels > 0) {
			swapChainDesc.SampleDesc.Count = 4;
			swapChainDesc.SampleDesc.Quality = supported4xMSAALevels;
		} else {
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
		}
	}
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = m_hwnd;
	swapChainDesc.Windowed = false;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	hResult = m_dxgiFactory->CreateSwapChain(m_device, &swapChainDesc, &m_swapChain);
	HREIF;

	hResult = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_swapChainBuffer);
	HREIF;

	hResult = m_device->CreateRenderTargetView(m_swapChainBuffer, NULL, &m_renderTargetView);
	HREIF;

	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	ZMEM(depthStencilBufferDesc);
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.Width = (UINT)settings.screenWidth;
	depthStencilBufferDesc.Height = (UINT)settings.screenHeight;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (settings.MSAAType == 0) {
		depthStencilBufferDesc.SampleDesc.Count = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
	} else if (settings.MSAAType == 2) {
		if (supported2xMSAALevels > 0) {
			depthStencilBufferDesc.SampleDesc.Count = 2;
			depthStencilBufferDesc.SampleDesc.Quality = supported2xMSAALevels;
		} else {
			depthStencilBufferDesc.SampleDesc.Count = 1;
			depthStencilBufferDesc.SampleDesc.Quality = 0;
		}
	} else {
		if (supported4xMSAALevels > 0) {
			depthStencilBufferDesc.SampleDesc.Count = 4;
			depthStencilBufferDesc.SampleDesc.Quality = supported4xMSAALevels;
		} else {
			depthStencilBufferDesc.SampleDesc.Count = 1;
			depthStencilBufferDesc.SampleDesc.Quality = 0;
		}
	}
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.MiscFlags = 0;

	hResult = m_device->CreateTexture2D(&depthStencilBufferDesc, NULL, &m_depthStencilBuffer);
	HREIF;

	hResult = m_device->CreateDepthStencilView(m_depthStencilBuffer, NULL, &m_depthStencilView);
	HREIF;

	m_context->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	D3D11_VIEWPORT viewport;
	ZMEM(viewport);
	viewport.Width = (float)settings.screenWidth;
	viewport.Height = (float)settings.screenHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	m_context->RSSetViewports(1, &viewport);

	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}
bool DirectX11::ApplySettings() {
	if (!AlreadyInitialized) {
		IFFAL(CreateGameWindow());
		IFFAL(CreateDirectX());
		AlreadyInitialized = true;
	} else {
		m_swapChain->SetFullscreenState(false, NULL);
		ReleaseCOM(m_swapChainBuffer);
		ReleaseCOM(m_renderTargetView);
		ReleaseCOM(m_depthStencilBuffer);
		ReleaseCOM(m_depthStencilView);
		ReleaseCOM(m_swapChain);
		
		IFFAL(CreateDirectX());
	}
	return true;
}
bool DirectX11::ChangeSettings(Settings &newSettings) {
	settings = newSettings;
	return ApplySettings();
}
int DirectX11::LookForClosestResolution() {
	int sX = GetSystemMetrics(SM_CXFULLSCREEN);
	int sY = GetSystemMetrics(SM_CYFULLSCREEN);
	int closestDistance = 9999999;
	int closestIndex = 0;
	for (int i = 0; i < 19; i++) {
		int currentDistance = abs(sX - SCREEN_RESOLUTIONS[i].width);
		if (currentDistance < closestDistance) {
			closestDistance = currentDistance;
			closestIndex = i;
		}
	}
	return closestIndex;
}
bool DirectX11::EnumerateAdapters() {
	IDXGIFactory1* factory;
	HRESULT hResult;
	hResult = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	HREIF;

	IDXGIAdapter* adapter;
	
	hResult = factory->EnumAdapters(0, &adapter);
	ReleaseCOM(adapter);

	while(!FAILED(hResult)) {
		ammountOfAdapters++;
		hResult = factory->EnumAdapters(ammountOfAdapters, &adapter);
		ReleaseCOM(adapter);
	}
	videoAdapters = new IDXGIAdapter*[ammountOfAdapters];
	for (int i = 0; i < ammountOfAdapters; i++) {
		factory->EnumAdapters(i, &videoAdapters[i]);
	}

	factory->Release();
	return true;
}
int DirectX11::LookForBestAdapter() {
	UINT mostMemory = 0;
	int index = 0;
	for (int i = 0; i < ammountOfAdapters; i++) {
		DXGI_ADAPTER_DESC adapterDesc;
		videoAdapters[i]->GetDesc(&adapterDesc);
		UINT memory = (UINT)adapterDesc.DedicatedVideoMemory;
		if (memory > mostMemory) {
			mostMemory = memory;
			index = i;
		}
	}
	return index;
}
void DirectX11::ClearScreen() {
	const float color[] = { 1.0f, 0.0f, 1.0f, 1.0f };
	m_context->ClearRenderTargetView(m_renderTargetView, color);
	m_context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, NEAR_Z, 0);
}
void DirectX11::PresentScene() {
	m_swapChain->Present(1, 0);
}