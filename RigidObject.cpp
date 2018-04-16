#include "RigidObject.h"

RigidObject::RigidObject(const Mesh *&inMesh, DirectX::XMMATRIX inWorld, ID3D11Device *&inGDevice, ID3D11DeviceContext *&inGDeviceContext)
	:Object(inMesh, inWorld, inGDevice, inGDeviceContext)
{
}

RigidObject::~RigidObject()
{
}

void RigidObject::operator=(const RigidObject & obj)
{
}
