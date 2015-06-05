#pragma once
#include "Object.h"

class Sphere :
	public Object
{
public:
	Sphere();
	Sphere(int sphere_id, int num);
	~Sphere();
	virtual void draw();
	static int sphereOffset;

protected:
	int sphere_id;
	int offset;
	int numVertex;
	int numY;
	float x; // size x
	float y; // size y
	int numTriangle;
	GLuint texture[2];
};

