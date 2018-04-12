//--------------------------------------------------------------------------------------
// BTH - Stefan Petersson 2014.
//--------------------------------------------------------------------------------------

#include <DirectXMath.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

//Time and input classes
#include"Time.h"
#include"Camera.h"

//WindowSize
const long WIDTH = 1600;
const long HEIGHT = 900;

//Move
float moveX = 0.0f;
float moveZ = 0.0f;

//Classes
Time time;
Camera camera(WIDTH, HEIGHT);

HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HRESULT CreateDirect3DContext(HWND wndHandle);

IDXGISwapChain* gSwapChain = nullptr;
ID3D11Device* gDevice = nullptr;
ID3D11DeviceContext* gDeviceContext = nullptr;
ID3D11InputLayout* gVertexLayout = nullptr;

//Buffers
ID3D11Buffer* gVertexBuffer = nullptr;
ID3D11Buffer* gExampleBuffer = nullptr;
ID3D11Buffer* lightSourceBuffer = nullptr;

//Views
ID3D11RenderTargetView* gBackbufferRTV = nullptr;
ID3D11DepthStencilView* depthView = nullptr;

//Shaders
ID3D11VertexShader* gVertexShader = nullptr;
ID3D11GeometryShader* gGeometryShader = nullptr;
ID3D11PixelShader* gPixelShader = nullptr;

//Transformation struct
struct valuesFromCpu {

	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;

};

//Light source struct
struct lightSourceValues {

	DirectX::XMFLOAT4 pos_intensity;
	DirectX::XMFLOAT4 color;

};

valuesFromCpu globalValues {

	DirectX::XMMatrixIdentity(),
	DirectX::XMMatrixIdentity(),
	DirectX::XMMatrixIdentity()

};

lightSourceValues lightSource{

	DirectX::XMFLOAT4(0.0f, 0.0f, -3.0f, 4.0f),	//Position & Intensity
	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)	//Color	

};

void CreateConstantBufferExample(){

	HRESULT hr = 0;

	// initialize the description of the buffer.
	D3D11_BUFFER_DESC exampleBufferDesc;
	exampleBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	exampleBufferDesc.ByteWidth = sizeof(valuesFromCpu);
	exampleBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	exampleBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	exampleBufferDesc.MiscFlags = 0;
	exampleBufferDesc.StructureByteStride = 0;

	// check if the creation failed for any reason
	hr = gDevice->CreateBuffer(&exampleBufferDesc, nullptr, &gExampleBuffer);

	if (FAILED(hr)){

		// handle the error, could be fatal or a warning...
		exit(-1);
	
	}
	
	//Initialize buffer for lightSource info
	D3D11_BUFFER_DESC lightSourceBufferDesc;
	lightSourceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightSourceBufferDesc.ByteWidth = sizeof(lightSourceValues);
	lightSourceBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightSourceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightSourceBufferDesc.MiscFlags = 0;
	lightSourceBufferDesc.StructureByteStride = 0;

	// check if the creation failed for any reason
	hr = gDevice->CreateBuffer(&lightSourceBufferDesc, nullptr, &lightSourceBuffer);

	if (FAILED(hr)) {

		// handle the error, could be fatal or a warning...
		exit(-1);

	}

}

void applyTransformations() {

	//World
	globalValues.world = DirectX::XMMatrixIdentity();

	//View
	globalValues.view = camera.getView();

	//Projection
	globalValues.projection = camera.getProj();

}

void createDepthStencil() {

	HRESULT hr = 0;

	//DepthStencil Desc
	D3D11_DEPTH_STENCIL_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.DepthEnable = true;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;

	//Stencil flags
	depthDesc.StencilEnable = false;
	depthDesc.StencilReadMask = D3D10_DEFAULT_STENCIL_READ_MASK;
	depthDesc.StencilWriteMask = D3D10_DEFAULT_STENCIL_WRITE_MASK;

	//frontface flags
	depthDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//backface flags
	depthDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Create depth stencil
	ID3D11DepthStencilState* depthState = nullptr;
	hr = gDevice->CreateDepthStencilState(&depthDesc, &depthState);

	if (FAILED(hr)) {

		exit(-1);

	}

	//Bind depth stencil state
	gDeviceContext->OMSetDepthStencilState(depthState, 1);

	depthState->Release();

	//Stencil texture and texture desc
	ID3D11Texture2D* pDepthStencil = nullptr;

	D3D11_TEXTURE2D_DESC depthTextureDesc;
	ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
	depthTextureDesc.Width = WIDTH;
	depthTextureDesc.Height = HEIGHT;
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTextureDesc.SampleDesc.Count = 1;
	depthTextureDesc.SampleDesc.Quality = 0;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureDesc.CPUAccessFlags = 0;
	depthTextureDesc.MipLevels = 0;

	//Create depthStencil texture
	hr = gDevice->CreateTexture2D(&depthTextureDesc, NULL, &pDepthStencil);

	if (FAILED(hr)) {

		exit(-1);

	}

	//Depth stencil view flags
	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;
	ZeroMemory(&depthViewDesc, sizeof(depthViewDesc));
	depthViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthViewDesc.Flags = 0;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Texture2D.MipSlice = 0;

	//Create stencil view
	hr = gDevice->CreateDepthStencilView(pDepthStencil, &depthViewDesc, &depthView);

	if (FAILED(hr)) {

		exit(-1);

	}

	//Release depthStencil texture
	pDepthStencil->Release();

}

void CreateShaders(){

	//create vertex shader
	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(
		L"Vertex.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		0,				// shader compile options			// here DEBUGGING OPTIONS
		0,				// effect compile options
		&pVS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gVertexShader);

	//create input layout (verified using vertex shader)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayout);
	
	// we do not need anymore this COM object, so we release it.
	pVS->Release();

	//create Geometry Shader
	ID3DBlob* pGS = nullptr;
	D3DCompileFromFile(
		L"Geometry.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"GS_main",		// entry point
		"gs_5_0",		// shader model (target)
		0,				// shader compile options			// here DEBUGGING OPTIONS
		0,				// effect compile options
		&pGS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);
	gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &gGeometryShader);

	// we do not need anymore this COM object, so we release it.
	pGS->Release();

	//create pixel shader
	ID3DBlob* pPS = nullptr;
	D3DCompileFromFile(
		L"Fragment.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		0,				// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);
	gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShader);
	
	// we do not need anymore this COM object, so we release it.
	pPS->Release();
	
}

void CreateTriangleData(){

	struct TriangleVertex{

		float x, y, z;
		float r, g, b;
	
	};

	TriangleVertex triangleVertices[6] = {

		//Triangle 1
		-0.25f, 0.25f, 0.0f,//v0 pos
		0.0f, 0.0f,	1.0f,	//v0 uv

		0.25f, -0.25f, 0.0f,//v1 pos
		1.0f, 1.0f,	0.0f,	//v1 uv

		-0.25f, -0.25f, 0.0f,//v2 pos
		0.0f, 1.0f,	0.0f,	//v2 uv

		//Triangle 2
		0.25f, -0.25f, 0.0f,//v0 pos
		1.0f, 1.0f,	0.0f,	//v0 uv

		-0.25f, 0.25f, 0.0f,//v1 pos
		0.0f, 0.0f,	1.0f,	//v1 uv

		0.25f, 0.25f, 0.0f, //v2 pos
		1.0f, 0.0f, 1.0f	//v2 uv

	};

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(triangleVertices);
	
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangleVertices;
	gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);
	
}

void SetViewport(){

	D3D11_VIEWPORT vp;
	vp.Width = WIDTH;
	vp.Height = HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	gDeviceContext->RSSetViewports(1, &vp);

}

void Render(HWND wndHandle){

	float clearColor[] = { 0, 0, 0, 1 };

	// set rendering state
	// if nothing changes, this does not have to be "re-done" every frame...
	gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(gGeometryShader, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);
	UINT32 vertexSize = sizeof(float) * 6;
	UINT32 offset = 0;

	gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBuffer, &vertexSize, &offset);
	gDeviceContext->IASetInputLayout(gVertexLayout);

	//Transformation matrices
	D3D11_MAPPED_SUBRESOURCE dataPtr;
	gDeviceContext->Map(gExampleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	memcpy(dataPtr.pData, &globalValues, sizeof(valuesFromCpu));
	gDeviceContext->Unmap(gExampleBuffer, 0);
	
	//Light source values
	gDeviceContext->Map(lightSourceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	memcpy(dataPtr.pData, &lightSource, sizeof(lightSourceValues));
	gDeviceContext->Unmap(lightSourceBuffer, 0);

	//Set resource to Geometry Shader and Pixel Shader
	gDeviceContext->GSSetConstantBuffers(0, 1, &gExampleBuffer);
	gDeviceContext->PSSetConstantBuffers(0, 1, &lightSourceBuffer);

	//Set primitive type
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// clear screen
	gDeviceContext->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	gDeviceContext->ClearRenderTargetView(gBackbufferRTV, clearColor);

	// draw geometry
	gDeviceContext->Draw(6, 0);

}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow){

	MSG msg = { 0 };

	HWND wndHandle = InitWindow(hInstance); //1. Skapa fönster

	if (wndHandle){

		CreateDirect3DContext(wndHandle); //2. Skapa och koppla SwapChain, Device och Device Context

		SetViewport(); //3. Sätt viewport

		CreateShaders(); //4. Skapa vertex- och pixel-shaders

		CreateTriangleData(); //5. Definiera triangelvertiser, 6. Skapa vertex buffer, 7. Skapa input layout

		CreateConstantBufferExample();

		ShowWindow(wndHandle, nCmdShow);

		//Init
		time.initTime();

		//init directInput
		camera.initDI(hInstance, wndHandle);

		//Apply values to matrices
		applyTransformations();

		while (WM_QUIT != msg.message){

			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else{

				//Update deltaTime
				time.update(wndHandle);

				//Update Input
				camera.getInput();

				//Update camera
				camera.update(time.getDeltaTime());

				//Update matrices
				applyTransformations();

				Render(wndHandle); //8. Rendera

				gSwapChain->Present(0, 0); //9. Växla front- och back-buffer
			
			}

		}

		//Release resources
		gVertexBuffer->Release();

		gVertexLayout->Release();
		gVertexShader->Release();
		gGeometryShader->Release();
		gPixelShader->Release();

		depthView->Release();
		gBackbufferRTV->Release();
		gSwapChain->Release();
		gDevice->Release();
		gDeviceContext->Release();
		DestroyWindow(wndHandle);
	}

	return (int)msg.wParam;

}

HWND InitWindow(HINSTANCE hInstance){

	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"TEMP";
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0, 0, WIDTH, HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND handle = CreateWindow(
		L"TEMP",
		L"TEMP",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	return handle;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

HRESULT CreateDirect3DContext(HWND wndHandle){

	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = wndHandle;                           // the window to be used
	scd.SampleDesc.Count = 1;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

															// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&gSwapChain,
		&gDevice,
		NULL,
		&gDeviceContext);

	if (SUCCEEDED(hr)){

		createDepthStencil();

		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = nullptr;
		gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		gDevice->CreateRenderTargetView(pBackBuffer, NULL, &gBackbufferRTV);
		pBackBuffer->Release();

		// set the render target as the back buffer
		gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTV, depthView);
	}
	return hr;
}