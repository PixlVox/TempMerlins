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
	ID3D11DeviceContext * deviceContext;
	ObjectImporter * objImporter;
	std::vector<Mesh>* meshes;
	std::string fileName;
	DirectX::XMMATRIX standardWorld;

	void createObjects();
public:
	Level(ID3D11Device *& in_device, ID3D11DeviceContext *& in_dContext, ObjectImporter *& importer);
	~Level();



	bool initialize();
};

#endif // !LEVEL_H
