#ifndef STATICOBJECT_H
#define STATICOBJECT_H
#include "Object.h"

class StaticObject : public Object
{
private:

public:
	StaticObject(const Mesh &inMesh, DirectX::XMMATRIX inWorld, ID3D11Device &inGDevice, ID3D11DeviceContext &inGDeviceContext);
	virtual ~StaticObject();
	void operator=(const StaticObject& obj);
};

#endif
