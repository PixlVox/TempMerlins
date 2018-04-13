#include "Level.h"

Level::Level()
{
	this->fileName = "Obj//Test.obj";
	this->meshes = nullptr;
	this->standardWorld = DirectX::XMMatrixIdentity();
	this->standardWorld *= DirectX::XMMatrixScaling(1000, 1000, 1000);
}

Level::~Level()
{
	this->deviceContext = nullptr;
	this->device = nullptr;
	this->objImporter = nullptr;
	this->sceneObjects.clear();
	this->fileName.clear();
}

bool Level::initialize(ID3D11Device * in_device, ID3D11DeviceContext * in_dContext, ObjectImporter * importer)
{
	//start up a scene
	this->device = in_device;
	this->deviceContext = in_dContext;
	this->objImporter = importer;

	bool rValue = false;
	rValue = this->objImporter->importModel(this->fileName, this->meshes);
	if (rValue)
	{
		//hantera object
		this->createObjects();
	}

	return rValue;
}

std::vector<Object>* Level::getObjects()
{
	return &this->sceneObjects;
}

void Level::createObjects()
{
	//reserver space for objects, one spot per mesh
	this->sceneObjects.reserve(this->meshes->capacity());

	//create objects and store to vector
	for (UINT32 i = 0; i < this->meshes->size(); i++)
	{
		Object nObject((*this->meshes)[i], this->standardWorld, this->device, this->deviceContext);
		nObject.setCollision(false);
		nObject.setVisibility(false);
		this->sceneObjects.push_back(nObject);
	}

	//reset meshes vector, no longer needed
	this->meshes->clear();
	delete this->meshes;
	this->meshes = nullptr;
}
