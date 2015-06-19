#pragma once
#include "Object.h"

class Skybox :
	public Object
{
public:
	Skybox();
	~Skybox();
	virtual void draw();
	virtual void reset();
	GLuint loadCubemap(vector<const GLchar*> faces);

protected:
	int offset;
	int numVertex;
	GLuint texture;
};

