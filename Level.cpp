#include "stdafx.h"
#include "Level.h"

Level::Level(ID3D11Device *& in_device, ObjectImporter *& importer, const std::string & fileName)
{
	this->device = in_device;
	this->objImporter = importer;
	this->fName = fileName;
}

Level::~Level()
{
}


void Level::createObjects()
{
	Object nObject;
}
