#ifndef STATICOBJECT_H
#define STATICOBJECT_H
#include "Object.h"

class StaticObject : public Object
{
private:

public:
	StaticObject(std::vector<Mesh> inMesh, XMMATRIX inWorld);
	virtual ~StaticObject();
	void operator=(const StaticObject& obj);
};

#endif
