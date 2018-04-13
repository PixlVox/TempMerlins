#ifndef ENGINE_H
#define ENGINE_H

#include"Camera.h"
#include"Level.h"

#include <d3dcompiler.h>

class Engine {

private:

	IDXGISwapChain * swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceC;
	ID3D11InputLayout* inputLayout;

	//Buffers
	ID3D11Buffer* constBufferMatrix;
	ID3D11Buffer* constBufferLight;

	//Views
	ID3D11RenderTargetView* backRTV;
	ID3D11DepthStencilView* depthView;

	//Shaders
	ID3D11VertexShader* vShader;
	ID3D11GeometryShader* gShader;
	ID3D11PixelShader* pShader;

	//Matrix struct
	struct Matrices {

		Matrix world;
		Matrix view;
		Matrix proj;

	};
	Matrices matrices;

	//Light Values struct
	struct Light {

		Vector4 pos_intensity;
		Vector4 col;

	};
	Light light;

	//Private functions
	void createConstBuffer();
	void createDepthStencil();
	void createShaders();
	void createContext(HWND* wndHandle);

	void setViewPort();
	void updateMatrices(Camera* cam, Object* object);
	void render(Object* object);

public:

	Engine();
	~Engine();

	void init(HWND* wHandle);
	void update(Camera* cam, std::vector<Object>* objects);

	//Get
	ID3D11Device* getDevice(void) const;
	ID3D11DeviceContext* getDeviceContext(void) const;

};

#endif // !ENGINE_H
