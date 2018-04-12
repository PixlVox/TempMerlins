#include "stdafx.h"
#include "Object.h"

void Object::createBuffers()
{
	///// Create vertex buffer /////
	D3D11_BUFFER_DESC vBufferDesc;
	memset(&vBufferDesc, 0, sizeof(vBufferDesc));
	vBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDesc.ByteWidth = sizeof(Mesh::Vertex) * this->mesh.vertexes.size;	

	D3D11_SUBRESOURCE_DATA vBufferData;
	vBufferData.pSysMem = this->mesh.vertexes.data;

	this->gDevice->CreateBuffer(&vBufferDesc, &vBufferData, &this->vBuffer);

	///// Create index buffer /////
	D3D11_BUFFER_DESC iBufferDesc;
	memset(&iBufferDesc, 0, sizeof(iBufferDesc));
	iBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	iBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	iBufferDesc.ByteWidth = sizeof(int) * this->mesh.indices.size;

	D3D11_SUBRESOURCE_DATA iBufferData;
	iBufferData.pSysMem = this->mesh.indices.data;

	this->gDevice->CreateBuffer(&iBufferDesc, &iBufferData, &iBuffer);

}

Object::Object(const Mesh &inMesh, DirectX::XMMATRIX inWorld, ID3D11Device &inGDevice)
{
	this->mesh = inMesh;
	this->world = inWorld;
	this->gDevice = &inGDevice;
	this->isVisible = false;
	this->isColliding = false;

	this->createBuffers();
	this->drawInfo = { this->vBuffer, this->iBuffer };
}

Object::~Object()
{	
	vBuffer->Release();
	iBuffer->Release();
}

void Object::operator=(const Object & obj)
{
	this->mesh = obj.mesh;
	this->world = obj.world;
}

void Object::setMesh(Mesh &inMesh)
{
	this->mesh = inMesh;
}

void Object::setWorldMatrix(DirectX::XMMATRIX inWorld)
{
	this->world = inWorld;
}

void Object::setVisibility(bool inIsVisible)
{
	this->isVisible = inIsVisible;
}

void Object::setCollision(bool inIsColliding)
{
	this->isColliding = inIsColliding;
}

Mesh Object::getMesh()
{
	return this->mesh;
}

DirectX::XMMATRIX Object::getWorldMatrix()
{
	return this->world;
}

ID3D11Buffer * Object::getVBuffer()
{
	return this->vBuffer;
}

ID3D11Buffer * Object::getIBuffer()
{
	return this->iBuffer;
}

Object::DrawInformation Object::getDrawInfo()
{
	return this->drawInfo;
}

bool Object::getVisibility()
{
	return this->isVisible;
}

bool Object::getCollision()
{
	return this->isColliding;
}
