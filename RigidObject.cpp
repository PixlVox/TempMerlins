#include "RigidObject.h"

RigidObject::RigidObject(const Mesh &inMesh, const DirectX::XMMATRIX &inWorld, ID3D11Device *&inGDevice)
	:Object(inMesh, inWorld, inGDevice)
{
}

RigidObject::~RigidObject()
{
}

void RigidObject::operator=(const RigidObject & obj)
{
}
