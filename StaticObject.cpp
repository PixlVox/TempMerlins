#include "StaticObject.h"

StaticObject::StaticObject(const Mesh &inMesh, DirectX::XMMATRIX inWorld, ID3D11Device &inGDevice, ID3D11DeviceContext &inGDeviceContext)
	:Object(inMesh, inWorld, inGDevice, inGDeviceContext)
{
}

StaticObject::~StaticObject()
{
}

void StaticObject::operator=(const StaticObject & obj)
{
}
