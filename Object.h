#ifndef OBJECT_H
#define OBJECT_H
#include "Mesh.h"
#include<d3d11.h>
#include<DirectXMath.h>

class Object
{
public:
	struct DrawInformation
	{
		ID3D11Buffer* InfoVBuffer, *InfoIBuffer;
	};

private:
	Mesh mesh;
	DirectX::XMMATRIX world;
	ID3D11Buffer* vBuffer;
	ID3D11Buffer* iBuffer;
	ID3D11Device* gDevice;
	DrawInformation drawInfo;	
	bool isVisible;
	bool isColliding;
	void createBuffers();

public:
	Object(const Mesh &inMesh, DirectX::XMMATRIX inWorld, ID3D11Device &inGDevice);
	virtual ~Object();
	void operator=(const Object& obj);

	void setMesh(Mesh &inMesh);
	void setWorldMatrix(DirectX::XMMATRIX inWorld);
	void setVisibility(bool inIsVisible);
	void setCollision(bool inIsColliding);

	Mesh getMesh();
	DirectX::XMMATRIX getWorldMatrix();
	ID3D11Buffer* getVBuffer();
	ID3D11Buffer*getIBuffer();
	DrawInformation getDrawInfo();
	bool getVisibility();
	bool getCollision();
};

#endif
