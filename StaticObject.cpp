#include "StaticObject.h"

StaticObject::StaticObject(const Mesh &inMesh, const DirectX::XMMATRIX &inWorld, ID3D11Device *&inGDevice)
	:Object(inMesh, inWorld, inGDevice)
{
}

StaticObject::~StaticObject()
{
}

void StaticObject::operator=(const StaticObject & obj)
{
}
