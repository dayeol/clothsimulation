#pragma once
#include "main.h"
#include "Rocket.h"
#include <iostream>
#include <list>

class Control
{
public:
	Control();
	~Control();
	void setWindowID(int winid);
	void specialKeyDown(int key, int x, int y);
	void specialKeyUp(int key, int x, int y);
	void keyDown(unsigned char key, int x, int y);
	void keyUp(unsigned char key, int x, int y);
	void mouse(GLint, GLint, GLint, GLint);
	void mouseMove(int x, int y);
	//animation control
	GLfloat animationSpeed;
	GLfloat revolutionSpeedFactor;
	GLfloat rotationSpeedFactor;

	bool zoomingIn;
	bool zoomingOut;
	bool movingLeft;
	bool movingRight;
	bool movingUp;
	bool movingDown;
	bool sphereMovingLeft;
	bool sphereMovingRight;

	float sphereX;
	mat4 sphere;

	void launchRocketOnCountdown();
	void launchRocket();
	void reuseRocket();
	//rocket control
	int rocketCountdown;

	bool isWireframe;
	bool isWind;
	bool isPin;
	GLboolean doAnimation;
private:

	bool isDragging;
	int dragOriginX;
	int dragOriginY;
	float original3dT;
	float original3dW;
	float cameraOriginx;
	float cameraOriginy;
	float cameraOriginz;

	//Window ID
	int window_id;
	
	//rocket control
	std::list<Rocket*> rockets;
};

extern Control controller;