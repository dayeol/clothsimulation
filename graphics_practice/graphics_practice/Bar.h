#pragma once
#include "Object.h"

class Bar :
	public Object
{
public:
	Bar();
	~Bar();
	virtual void draw();
	virtual void reset();

protected:
	int offset;
	int numVertex;
	GLuint texture;
};

