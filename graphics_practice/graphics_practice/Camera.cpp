#include "Camera.h"

Camera camera;

Camera::Camera()
{
	x = 0;
	y = 0;
	z = 2.0f;
	px = 0;
	py = 0;
	pz = -1;
	zoom = 1.0f;
	mode = PERSPECTIVE;
	//3D

	_3dx = 0;
	_3dy = 0;
	_3dz = 0;
	_3ddist = 5.0f;
	_3dT = 45;
	_3dW = 90;
	recalculate3D();
}

void Camera::recalculate3D()
{
	if (_3ddist >= 30.0)
	{
		_3ddist = 30.0;
	}
	if (_3ddist <= 0.2)
	{
		_3ddist = 0.2;
	}
	if (_3dT >= 360)
	{
		_3dT -= 360;
	}
	if (_3dW >= 360)
	{
		_3dW -= 360;
	}
	if (_3dT < 0)
	{
		_3dT += 360;
	}
	if (_3dW < 0)
	{
		_3dW += 360;
	}

	//printf("_3ddist: %f, _3dW: %f, _3dT: %f\n", _3ddist, _3dW, _3dT);
	px = _3ddist*sin(_3dT*M_PI / 180)*sin(_3dW*M_PI / 180);
	py = _3ddist*sin(_3dT*M_PI / 180)*cos(_3dW*M_PI / 180);
	pz = _3ddist*cos(_3dT*M_PI / 180);
	vec3 target = vec3(_3dx, _3dy, _3dz);
	//printv(target);
	vec3 eye = vec3(_3dx + px, _3dy + py, _3dz + pz);
	//printv(eye);
	lookat3D = LookAtRH(eye, target, vec3(0, 0, 100));
}

void Camera::zoomIn()
{
	float maxZoom = 0.05f;
	if (zoom >= maxZoom)
		zoom *= 0.9f;
}

void Camera::zoomOut()
{
	zoom *= 1.1f;
}

void Camera::move(direction to)
{
	vec3 viewingDirection = -vec3(px, py, pz);
	vec3 right = cross(viewingDirection, vec3(0, 0, 100));
	//printv(right);
	float moveAmount = 0.002;
	switch (to)
	{
	case UP:
		if (mode == ORTHOGONAL)
			y += moveAmount;
		else
		{
			_3ddist -= moveAmount * 100;
			recalculate3D();
		}
		break;
	case DOWN:
		if (mode == ORTHOGONAL)
			y -= moveAmount;
		else
		{
			_3ddist += moveAmount * 100;
			recalculate3D();
		}
		break;
	case LEFT:
		if (mode == ORTHOGONAL)
		{
			x -= moveAmount;
		}
		else
		{
			_3dx -= moveAmount * (right.x);
			_3dy -= moveAmount * (right.y);
			//_3dz -= right.z * moveAmount * 0.1;
			recalculate3D();
		}
		break;
	case RIGHT:
		if (mode == ORTHOGONAL)
		{
			x += moveAmount;
		}
		else
		{
			_3dx += moveAmount * (right.x);
			_3dy += moveAmount * (right.y);
			//_3dz += right.z * moveAmount * 0.1;
			recalculate3D();
		}
		break;
	default:
		break;
	}
}

void Camera::toggleMode()
{
	if (mode == PERSPECTIVE)
		mode = ORTHOGONAL;
	else
		mode = PERSPECTIVE;
}

Camera::~Camera()
{
}
mat4 Camera::projection()
{
	if (mode == ORTHOGONAL)
	{
		GLfloat left = -whratio, right = whratio, bottom = -1, top = 1, zNear = 1, zFar = -1;
		mat4 projection = Ortho(zoom * left, zoom * right, zoom * bottom, zoom * top, zNear, zFar);
		return projection;
	}
	else
	{
		mat4 projection = Perspective(45, whratio, 0.1, 50.0);
		return projection;
	}
}

mat4 Camera::lookAt()
{
	if (mode == ORTHOGONAL)
		return Translate(-x, -y, 0);
	else
	{
		return lookat3D;
	}
}