#include "Skybox.h"

#include <vector>

Skybox::Skybox(int x, int y, int z, ObjModel* model)
{
	Skybox::x = x;
	Skybox::y = y;
	Skybox::z = z;
	Skybox::model = model;
}


Skybox::~Skybox()
{
}

void Skybox::draw()
{
	//model->draw();
}
