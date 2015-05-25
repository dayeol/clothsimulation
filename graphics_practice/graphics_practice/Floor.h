#pragma once
#include "Object.h"

class Floor :
	public Object
{
public:
	Floor();
	Floor(float _x, float _y);
	~Floor();
	virtual void draw(bool isSelect);
	
protected:
	float x; // size x
	float y; // size y
	int size;
};

