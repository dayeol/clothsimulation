#pragma once
#include "Object.h"
#include <time.h>

class Rocket :
	public Object
{
public:
	Rocket();
	void draw();
	bool isExhausted() { return fuel == 0; };
	void relaunch();
private:
	void updatePosition();
	int fuel;
	float speed;
	//position
	bool rightTurn;
	GLfloat x;
	GLfloat y;
	GLfloat direction; //represented by 0 <= angle < 360
};

