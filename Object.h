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
	Object(const Mesh &inMesh, const DirectX::XMMATRIX &inWorld, ID3D11Device *&inGDevice);
	virtual ~Object();
	void operator=(const Object& obj);

	void setMesh(const Mesh &inMesh);
	void setWorldMatrix(const DirectX::XMMATRIX &inWorld);
	void setVisibility(const bool &inIsVisible);
	void setCollision(const bool &inIsColliding);

	Mesh getMesh()const;
	DirectX::XMMATRIX getWorldMatrix()const;
	ID3D11Buffer* getVBuffer()const;
	ID3D11Buffer* getIBuffer()const;
	DrawInformation getDrawInfo()const;
	bool getVisibility()const;
	bool getCollision()const;
};

#endif
