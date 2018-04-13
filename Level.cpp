#include "Level.h"

Level::Level(ID3D11Device *& in_device, ObjectImporter *& importer, const std::string & fileName)
{
	this->device = in_device;
	this->objImporter = importer;
	this->fileName = fileName;
}

Level::~Level()
{
}


void Level::createObjects()
{
	//Object nObject;
}
