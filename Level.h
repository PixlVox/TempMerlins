#ifndef LEVEL_H
#define LEVEL_H
#include"ObjectImporter.h"
#include"Object.h"

class Level
{
public:

	std::vector<Object> sceneObjects;
private:
	ID3D11Device * device;
	ObjectImporter * objImporter;
	std::string fileName;


	void createObjects();
public:
	Level(ID3D11Device* &in_device, ObjectImporter* &importer, const std::string &fileName);
	~Level();


};

#endif // !LEVEL_H
