#pragma once
#include "Object.h"
#include "SOIL.h"
class Planet :
	public Object
{
public:
	Planet();
	Planet(vec4 _color, float _size, float _distance, float revolutionSpeed, float rotationSpeed, char* textureName);
	~Planet();
	virtual void draw(bool isSelect);
	GLuint loadImage(const string imagePath);
	void updateAngle();
	static unsigned int numberOfPlanets;
	static unsigned int planetVertices;
	static unsigned int orbitVertices;
	GLuint texture;
	
protected:
	int planetID;
	mat4 color;
	float size;
	float distance;
	float revSpeed;
	float rotSpeed;

	float revAngle;
	float rotAngle;
};

