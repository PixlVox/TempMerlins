#ifndef RIGIDOBJECT_H
#define RIGIDOBJECT_H

class RigidObject : public Object
{
private:

public:
	RigidObject(std::vector<Mesh> inMesh, XMMATRIX inWorld);
	virtual ~RigidObject();
	void operator=(const RigidObject& obj);
};
#endif
