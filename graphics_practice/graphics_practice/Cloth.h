#pragma once
#include "Object.h"

class Cloth :
	public Object
{
public:
	Cloth();
	Cloth(float _x, float _y, int numX, int numY);
	virtual void draw();

protected:
	int numX;
	int numY;
	float x; // size x
	float y; // size y
	int numTriangle;
	GLuint texture[2];
};

