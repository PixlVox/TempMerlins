#include "BoxModelHandler.h"

BoxModelHandler::BoxModelHandler(Geometry* in_Geometry)
{
	this->geometry = in_Geometry;
	this->boxModels = new BoxModel[];

	this->initiateBoxModels();
}
