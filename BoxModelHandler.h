#ifndef BOXMODELHANDLER_H
#define BOXMODELHANDLER_H
#include "BoxModel.h"
class BoxModelHandler
{
private:
	Geometry * geometry;
	BoxModel * boxModels;	

	void initiateBoxModels();
public:
	BoxModelHandler(Geometry* in_Geometry);
	~BoxModelHandler();

	void update();
	void release();
	void createModel();
	void render();
};
#endif
