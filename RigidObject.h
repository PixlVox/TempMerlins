#ifndef RIGIDOBJECT_H
#define RIGIDOBJECT_H
#include "Object.h"
class RigidObject : public Object
{
private:

public:
	RigidObject(const Mesh &inMesh, const DirectX::XMMATRIX &inWorld, ID3D11Device *&inGDevice);
	virtual ~RigidObject();
	void operator=(const RigidObject& obj);
};
#endif
