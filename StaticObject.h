#ifndef STATICOBJECT_H
#define STATICOBJECT_H
#include "Object.h"

class StaticObject : public Object
{
private:

public:
	StaticObject(const Mesh &inMesh, const DirectX::XMMATRIX &inWorld, ID3D11Device *&inGDevice);
	virtual ~StaticObject();
	void operator=(const StaticObject& obj);
};

#endif
