#pragma once
#include "Planet.h"

class Earth :
	public Planet
{
public:
	Earth();
	Earth(vec4 _color, float _size, float _distance, float revolutionSpeed, float rotationSpeed);
	GLuint addEarthTexture(char* textureFile);
	~Earth();
	virtual void draw(bool isSelect);
	GLuint tex[5];
};

