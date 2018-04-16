#include"Engine.h"

Engine::Engine() {

	//Engine stuff
	this->swapChain = nullptr;
	this->device = nullptr;
	this->deviceC = nullptr;

	//Shaders
	this->vShader = nullptr;
	this->gShader = nullptr;
	this->pShader = nullptr;
	
	//Object stuff
	this->inputLayout = nullptr;

	//Light buffer
	this->constBufferMatrix = nullptr;
	this->constBufferLight = nullptr;

	//Init matrix values
	this->matrices.world = Matrix::Identity;
	this->matrices.view = Matrix::Identity;
	this->matrices.proj = Matrix::Identity;

	//Init light values
	this->light.pos_intensity = Vector4(0.0f, 0.0f, -3.0f, 4.0f);
	this->light.col = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

}

Engine::~Engine() {

	if (this->swapChain) {

		this->swapChain->Release();

	}

	if (this->device) {

		this->device->Release();

	}

	if (this->deviceC) {

		this->deviceC->Release();

	}

	if (this->constBufferMatrix) {

		this->constBufferMatrix->Release();

	}

	if (this->constBufferLight) {

		this->constBufferLight->Release();

	}

	if (this->vShader) {

		this->vShader->Release();

	}

	if (this->gShader) {

		this->gShader->Release();

	}

	if (this->pShader) {

		this->pShader->Release();

	}

}

void Engine::init(HWND* wHandle) {
	
	//Create device, deviceC, swapChain
	this->createContext(wHandle);

	//Set viewport
	this->setViewPort();

	//Create shaders
	this->createShaders();

	//Create const buffers
	this->createConstBuffer();

}

void Engine::update(Camera* cam, std::vector<Object>* objects) {
	
	//Render
	for (int i = 0; i < objects->size(); i++) {

		//Update matrices
		this->updateMatrices(cam, &objects->at(i));

		//Render
		this->render(&objects->at(i));

	}

	//Update swapChain
	this->swapChain->Present(0, 0);

}

void Engine::updateMatrices(Camera* cam, Object* object) {

	this->matrices.world = object->getWorldMatrix();
	this->matrices.view = cam->getView();
	this->matrices.proj = cam->getProj();

}

void Engine::createContext(HWND* wndHandle) {

	//SwapChain Desc
	DXGI_SWAP_CHAIN_DESC scD;
	ZeroMemory(&scD, sizeof(DXGI_SWAP_CHAIN_DESC));
	scD.BufferCount = 1;                                    // one back buffer
	scD.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scD.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scD.OutputWindow = *wndHandle;                           // the window to be used
	scD.SampleDesc.Count = 1;                               // how many multisamples
	scD.Windowed = TRUE;                                    // windowed/full-screen mode

	//Create device, deivceC & swapChain
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scD,
		&this->swapChain,
		&this->device,
		NULL,
		&this->deviceC);

	if (SUCCEEDED(hr)) {

		createDepthStencil();

		//BackBuffer address
		ID3D11Texture2D* pBackBuffer = nullptr;
		this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		//Create renderTarget
		this->device->CreateRenderTargetView(pBackBuffer, NULL, &this->backRTV);
		pBackBuffer->Release();

		//Set renderTarget
		this->deviceC->OMSetRenderTargets(1, &this->backRTV, depthView);

	}

}

void Engine::createConstBuffer() {

	HRESULT hr = 0;

	//Buffer Desc
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(Matrix);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	//Create buffer
	hr = this->device->CreateBuffer(&bufferDesc, nullptr, &this->constBufferMatrix);

	if (FAILED(hr)) {

		exit(-1);

	}

	//Buffer Desc
	D3D11_BUFFER_DESC bufferDescL;
	bufferDescL.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescL.ByteWidth = sizeof(Light);
	bufferDescL.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescL.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDescL.MiscFlags = 0;
	bufferDescL.StructureByteStride = 0;

	//Create buffer
	hr = this->device->CreateBuffer(&bufferDescL, nullptr, &this->constBufferLight);

	if (FAILED(hr)) {

		exit(-1);

	}

}

void Engine::createDepthStencil() {

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

	//Frontface flags
	depthDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Backface flags
	depthDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Create depth stencil
	ID3D11DepthStencilState* depthState = nullptr;
	hr = this->device->CreateDepthStencilState(&depthDesc, &depthState);

	if (FAILED(hr)) {

		exit(-1);

	}

	//Bind depth stencil state
	this->deviceC->OMSetDepthStencilState(depthState, 1);

	depthState->Release();

	//Stencil texture and texture desc
	ID3D11Texture2D* pDepthStencil = nullptr;

	D3D11_TEXTURE2D_DESC depthTextureDesc;
	ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
	depthTextureDesc.Width = WIN_WIDTH;
	depthTextureDesc.Height = WIN_HEIGHT;
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
	hr = this->device->CreateTexture2D(&depthTextureDesc, NULL, &pDepthStencil);

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
	hr = this->device->CreateDepthStencilView(pDepthStencil, &depthViewDesc, &depthView);

	if (FAILED(hr)) {

		exit(-1);

	}

	//Release depthStencil texture
	pDepthStencil->Release();

}

void Engine::createShaders() {

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

	this->device->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &this->vShader);

	//create input layout (verified using vertex shader)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	this->device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &this->inputLayout);

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
	this->device->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &this->gShader);

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
	this->device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &this->pShader);

	// we do not need anymore this COM object, so we release it.
	pPS->Release();

}

void Engine::setViewPort() {

	D3D11_VIEWPORT vp;
	vp.Width = WIN_WIDTH;
	vp.Height = WIN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	this->deviceC->RSSetViewports(1, &vp);

}

void Engine::render(Object* object) {

	//Standard pixel color
	float clearColor[] = { 0.25, 0.5, 0.75, 1 };

	//Set rendering state
	this->deviceC->VSSetShader(this->vShader, nullptr, 0);
	this->deviceC->HSSetShader(nullptr, nullptr, 0);
	this->deviceC->DSSetShader(nullptr, nullptr, 0);
	this->deviceC->GSSetShader(this->gShader, nullptr, 0);
	this->deviceC->PSSetShader(this->pShader, nullptr, 0);
	UINT32 vertexSize = sizeof(float) * 6;
	UINT32 offset = 0;

	//Bind vertex buffer
	ID3D11Buffer* tempVBuffer = object->getVBuffer();
	this->deviceC->IASetVertexBuffers(0, 1, &tempVBuffer, &vertexSize, &offset);

	//Bind index buffer
	//ID3D11Buffer* tempIBuffer = object->getIBuffer();
	//this->deviceC->IASetIndexBuffer(tempIBuffer, DXGI_FORMAT_R32_UINT, offset);

	//Set input layout
	this->deviceC->IASetInputLayout(this->inputLayout);

	//Matrices
	D3D11_MAPPED_SUBRESOURCE dataPtr;
	this->deviceC->Map(this->constBufferMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	memcpy(dataPtr.pData, &this->matrices, sizeof(Matrices));
	this->deviceC->Unmap(this->constBufferMatrix, 0);

	//Light source values
	this->deviceC->Map(this->constBufferLight, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	memcpy(dataPtr.pData, &this->light, sizeof(Light));
	this->deviceC->Unmap(this->constBufferLight, 0);

	//Set resource to Geometry Shader and Pixel Shader
	this->deviceC->GSSetConstantBuffers(0, 1, &this->constBufferMatrix);
	this->deviceC->PSSetConstantBuffers(0, 1, &this->constBufferLight);

	//Set primitive type
	this->deviceC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// clear screen
	this->deviceC->ClearDepthStencilView(this->depthView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	this->deviceC->ClearRenderTargetView(this->backRTV, clearColor);

	// draw geometry
	this->deviceC->Draw(6, 0);

}

ID3D11Device* Engine::getDevice(void) const{

	return this->device;

}

ID3D11DeviceContext* Engine::getDeviceContext(void) const{

	return this->deviceC;

}