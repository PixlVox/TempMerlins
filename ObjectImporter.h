#ifndef OBJECTIMPORTER_H
#define OBJECTIMPORTER_H
#include<assimp\Importer.hpp>
#include"Mesh.h"

class ObjectImporter
{
private:
	Assimp::Importer importer;

	void debugString(const LPCWSTR &in_string, const aiReturn &in_param);
public:
	ObjectImporter();
	~ObjectImporter();

	bool importModel(const std::string &fname, std::vector<Mesh>* &returnMesh);


};
#endif // !OBJECTIMPORTER_H
