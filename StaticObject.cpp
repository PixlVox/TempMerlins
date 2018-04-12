#include "stdafx.h"
#include "StaticObject.h"

StaticObject::StaticObject(std::vector<Mesh> inMesh, XMMATRIX inWorld)
	:Object(inMesh, inWorld)
{
}

StaticObject::~StaticObject()
{
}

void StaticObject::operator=(const StaticObject & obj)
{
}
