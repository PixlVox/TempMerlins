#include "stdafx.h"
#include "RigidObject.h"

RigidObject::RigidObject(std::vector<Mesh> inMesh, XMMATRIX inWorld)
	:Object(inMesh, inWorld)
{
}

RigidObject::~RigidObject()
{
}

void RigidObject::operator=(const RigidObject & obj)
{
}
