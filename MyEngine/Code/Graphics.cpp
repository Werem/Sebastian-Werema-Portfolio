#include "Graphics.h"


Graphics::Graphics()
{
}


Graphics::~Graphics()
{
}

bool Graphics::Initialize(HINSTANCE hInstance) {
	m_hinstance = hInstance;
	testMesh = nullptr;
	testShader = nullptr;
	mainTexture = nullptr;
	fonts = nullptr;
	myText = nullptr;
	spriteShader = nullptr;
	renderTexture = nullptr;
	IFFAL(InitializeWindow());
	IFFAL(InitializeDirectX11());
#ifdef TESTING_DRAWING
	IFFAL(testInit());
#endif
	return true;
}
void Graphics::Shutdown() {
	ShutdownWindow();
	ShutdownDirectx11();
	ReleaseCOM(testBuffer);
	SHUTDOWN(testMesh);
	SHUTDOWN(testShader);
	SHUTDOWN(mainTexture);
	SHUTDOWN(fonts);
	SHUTDOWN(spriteShader);
	SHUTDOWN(myText);
	SHUTDOWN(renderTexture);
}
bool Graphics::InitializeWindow() {
	m_screenWidth = (float)GetSystemMetrics(SM_CXSCREEN);
	m_screenHeight = (float)GetSystemMetrics(SM_CYSCREEN); //Set private window variables

	WNDCLASSEX wc;
	ZMEM(wc);
	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpszClassName = L"My Game";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor = LoadCursor(m_hinstance, IDC_ARROW);
	wc.hIcon = LoadIcon(m_hinstance, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hInstance = m_hinstance;
	wc.lpfnWndProc = WndProc;

	IFFAL(RegisterClassEx(&wc));

	m_hwnd = CreateWindowEx(NULL, L"My Game", L"DirectX11 Window", WS_OVERLAPPEDWINDOW, 0, 0, (int)m_screenWidth, (int)m_screenHeight, NULL, NULL, m_hinstance, NULL);
	IFEXIST(m_hwnd);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
	//ShowCursor(false);
	
	return true;
}
void Graphics::ShutdownWindow() {
	DestroyWindow(m_hwnd);
	UnregisterClass(L"My Game", m_hinstance);
}
bool Graphics::InitializeDirectX11() {
	HRESULT hResult;
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
	#ifdef DEBUG_MODE
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif
	
	hResult = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, NULL, NULL, D3D11_SDK_VERSION, &m_device, &m_featureLevel, &m_deviceContext);
	HREIF;

	if (m_featureLevel != D3D_FEATURE_LEVEL_11_0) {
		SIMPLE_MESSAGEBOX_ERROR(L"DirectX 11 Feature Level not supported");
		return false;
	}

	hResult = m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &MSAAx4Level);
	HREIF;
	hResult = m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 2, &MSAAx2Level);
	HREIF;

	IDXGIDevice* dxgiDevice = 0;
	hResult = m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	HREIF;
	IDXGIAdapter* dxgiAdapter = 0;
	hResult = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	HREIF;
	IDXGIFactory* dxgiFactory = 0;
	hResult = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
	HREIF;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZMEM(swapChainDesc);
	swapChainDesc.BufferDesc.Width = (UINT)m_screenWidth;
	swapChainDesc.BufferDesc.Height = (UINT)m_screenHeight;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = m_hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	hResult = dxgiFactory->CreateSwapChain(m_device, &swapChainDesc, &m_swapChain);
	HREIF;

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	ID3D11Texture2D* backBuffer;
	hResult = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	HREIF;
	hResult = m_device->CreateRenderTargetView(backBuffer, 0, &m_renderTargetView);
	HREIF;
	ReleaseCOM(backBuffer);
	
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZMEM(depthStencilDesc);
	depthStencilDesc.Width = (UINT)m_screenWidth;
	depthStencilDesc.Height = (UINT)m_screenHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	hResult = m_device->CreateTexture2D(&depthStencilDesc, NULL, &m_depthStencilBuffer);
	HREIF;
	hResult = m_device->CreateDepthStencilView(m_depthStencilBuffer, NULL, &m_depthStencilView);
	HREIF;

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	D3D11_VIEWPORT viewport;
	ZMEM(viewport);
	viewport.Width = m_screenWidth;
	viewport.Height = m_screenHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	m_deviceContext->RSSetViewports(1, &viewport);

	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
}
void Graphics::ShutdownDirectx11() {
	ReleaseCOM(m_device);
	ReleaseCOM(m_deviceContext);
	ReleaseCOM(m_swapChain);
	ReleaseCOM(m_renderTargetView);
	ReleaseCOM(m_depthStencilBuffer);
	ReleaseCOM(m_depthStencilView);
}
bool Graphics::Render() {
	if (PeekMessage(&m_msg, m_hwnd, 0, 0, PM_REMOVE)) {
		if (m_msg.message == WM_QUIT) return false;
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
	}
	HRESULT hResult;
	float backgroundColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, nearZ, 0);
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, backgroundColor);
#ifdef TESTING_DRAWING
	IFFAL(testDraw());
#endif
	hResult = m_swapChain->Present(1, 0);
	HREIF;
	return true;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_CLOSE: {
			PostQuitMessage(0);
			return 0;
		}
		case WM_QUIT: {
			PostQuitMessage(0);
			return 0;
		}
		case WM_KEYDOWN: {
			if (wParam == VK_ESCAPE) {
				MessageBox(NULL, L"Closing", L"It's", MB_OK);
				PostQuitMessage(0);
				return 0;
			}
		}
		default: {
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}
}
bool Graphics::testInit() {
	testShader = new Shader();
	D3D11_INPUT_ELEMENT_DESC inputDesc[3];
	//	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 12, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	//};
	inputDesc[0].AlignedByteOffset = 0;
	inputDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDesc[0].InputSlot = 0;
	inputDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[0].InstanceDataStepRate = 0;
	inputDesc[0].SemanticIndex = 0;
	inputDesc[0].SemanticName = "POSITION";
	inputDesc[1].AlignedByteOffset = 12;
	inputDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputDesc[1].InputSlot = 0;
	inputDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[1].InstanceDataStepRate = 0;
	inputDesc[1].SemanticIndex = 0;
	inputDesc[1].SemanticName = "COLOR";
	inputDesc[2].AlignedByteOffset = 28;
	inputDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputDesc[2].InputSlot = 0;
	inputDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc[2].InstanceDataStepRate = 0;
	inputDesc[2].SemanticIndex = 0;
	inputDesc[2].SemanticName = "TEXCOORD";
	IFFAL(testShader->Initialize(m_device, L"TestShader.vs", L"TestShader.ps", inputDesc, 3));
	struct Vertex {
		XMFLOAT3 position;
		XMFLOAT4 color;
		XMFLOAT2 uv;
		void Initialize(float x, float y, float z, float r, float g, float b, float u, float v) {
			position.x = x;
			position.y = y;
			position.z = z;
			color.x = r;
			color.y = g;
			color.z = b;
			color.w = 1.0f;
			uv.x = u;
			uv.y = v;
		}
	};
	Vertex* vertices = new Vertex[4];
	vertices[0].Initialize(-1000.5f, -3.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[1].Initialize(-1000.5f, 35.5f, 3000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[2].Initialize(1000.5f, 35.5f, 3000.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	vertices[3].Initialize(1000.5f, -3.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

	UINT16* indices = new UINT16[6];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	testMesh = new Mesh();
	IFFAL(testMesh->Initialize(m_device, (void*)vertices, indices, 6, sizeof(Vertex), 4, D3D11_USAGE_DEFAULT));

	testCamera = new Camera();
	testCamera->Initialize(m_screenWidth, m_screenHeight, 0.0f, 50.0f, 1.5f, 0.0f, 0.0f, 0.0f);

	struct ConstantBuffer {
		XMMATRIX mvpMatrix;
	};
	ConstantBuffer cBuffer;
	cBuffer.mvpMatrix = XMMatrixTranspose(testCamera->viewMatirx * testCamera->projectionMatrix);
	D3D11_BUFFER_DESC bufferDesc;
	ZMEM(bufferDesc);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth = sizeof(ConstantBuffer);
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA subData;
	ZMEM(subData);
	subData.pSysMem = &cBuffer;
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;

	HRESULT hResult = m_device->CreateBuffer(&bufferDesc, &subData, &testBuffer);
	HREIF;

	mainTexture = new Texture2D();
	std::vector<std::string> texturePaths;
	texturePaths.push_back("Checker");
	texturePaths.push_back("Dirt");
	IFFAL(mainTexture->Initialize(m_device, texturePaths));
	int indexOfArray = 0;

	spriteShader = new Shader();
	D3D11_INPUT_ELEMENT_DESC spriteElementDesc[2];
	//	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 12, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	//};
	spriteElementDesc[0].AlignedByteOffset = 0;
	spriteElementDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	spriteElementDesc[0].InputSlot = 0;
	spriteElementDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	spriteElementDesc[0].InstanceDataStepRate = 0;
	spriteElementDesc[0].SemanticIndex = 0;
	spriteElementDesc[0].SemanticName = "POSITION";
	spriteElementDesc[1].AlignedByteOffset = 12;
	spriteElementDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	spriteElementDesc[1].InputSlot = 0;
	spriteElementDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	spriteElementDesc[1].InstanceDataStepRate = 0;
	spriteElementDesc[1].SemanticIndex = 0;
	spriteElementDesc[1].SemanticName = "TEXCOORD";
	IFFAL(spriteShader->Initialize(m_device, L"SpriteShader.vs", L"SpriteShader.ps", spriteElementDesc, 2));

	fonts = new Font();
	IFFAL(fonts->Initialize(m_device));
	fonts->RecalculateFontData(m_screenWidth, m_screenHeight);

	myText = new Text();
	std::string toSay = "Font Test";
	IFFAL(myText->Initialize(m_device, fonts, -0.5f, 0.0f, 8.0f, 5.0f, toSay, 5, D3D11_USAGE_IMMUTABLE));

	wrapSampler = new TextureSampler();
	IFFAL(wrapSampler->Initialize(m_device, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER_MIN_MAG_MIP_LINEAR));

	m_deviceContext->PSSetSamplers(0, 1, &wrapSampler->samplerState);
	m_deviceContext->VSSetConstantBuffers(0, 1, &testBuffer);
	m_deviceContext->PSSetShaderResources(0, 1, &mainTexture->shaderResourceView);
	m_deviceContext->PSSetShaderResources(1, 1, &fonts->texture->shaderResourceView);
	return true;
}
bool Graphics::testDraw() {
	m_deviceContext->IASetVertexBuffers(0, 1, &testMesh->vertexBuffer, &testMesh->stride, &testMesh->offset);
	m_deviceContext->IASetIndexBuffer(testMesh->indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	m_deviceContext->PSSetShader(testShader->m_pixelShader, 0, 0);
	m_deviceContext->VSSetShader(testShader->m_vertexShader, 0, 0);
	m_deviceContext->IASetInputLayout(testShader->m_inputLayout);
	m_deviceContext->DrawIndexed(6, 0, 0);
	m_deviceContext->IASetVertexBuffers(0, 1, &myText->mesh->vertexBuffer, &myText->mesh->stride, &myText->mesh->offset);
	m_deviceContext->IASetIndexBuffer(myText->mesh->indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	m_deviceContext->PSSetShader(spriteShader->m_pixelShader, 0, 0);
	m_deviceContext->VSSetShader(spriteShader->m_vertexShader, 0, 0);
	m_deviceContext->IASetInputLayout(spriteShader->m_inputLayout);
	m_deviceContext->DrawIndexed(myText->indexAmmount, 0, 0);
	return true;
}