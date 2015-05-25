#pragma once
#include "Angel.h"

extern class Camera camera;

class Camera
{
public:
	Camera();
	~Camera();
	void zoomIn();
	void zoomOut();
	enum direction { UP, DOWN, LEFT, RIGHT };
	void move(direction to);
	void setRatio(int w, int h) { whratio = (float)w / h; };
	mat4 projection();
	mat4 lookAt();
	enum Mode { PERSPECTIVE, ORTHOGONAL };
	void toggleMode();
	GLfloat px, py, pz;
	GLfloat x;
	GLfloat y;
	GLfloat z;

	//3D viewing target;
	GLfloat _3dx;
	GLfloat _3dy;
	GLfloat _3dz;
	///
	GLfloat _3dT; // angle ( z-axis, proj )
	GLfloat _3dW; // angle ( x-axis, proj )
	GLfloat _3ddist; //distance from cam to target
	void recalculate3D();
	mat4 lookat3D;
private:
	GLfloat whratio; //width / height ratio = w/h

	GLfloat zoom;
	Mode mode;
};