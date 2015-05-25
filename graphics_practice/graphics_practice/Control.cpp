#include "Control.h"
#include "main.h"
#include "Camera.h"

Control controller;

Control::Control()
{
	doAnimation = true; 
	animationSpeed = 0.1f;
	revolutionSpeedFactor = 1.0f;
	rotationSpeedFactor = 1.0f;
	rocketCountdown = 1000;
	isDragging = false;
	dragOriginX = 0;
	dragOriginY = 0;
}

Control::~Control()
{
}

void Control::setWindowID(int winid)
{
	window_id = winid;
}

void Control::specialKeyDown(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		movingUp = true;
	if (key == GLUT_KEY_DOWN)
		movingDown = true;
	if (key == GLUT_KEY_LEFT)
		movingLeft = true;
	if (key == GLUT_KEY_RIGHT)
		movingRight = true; 
}
void Control::specialKeyUp(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		movingUp = false;
	if (key == GLUT_KEY_DOWN)
		movingDown = false;
	if (key == GLUT_KEY_LEFT)
		movingLeft = false;
	if (key == GLUT_KEY_RIGHT)
		movingRight = false;

	printf("x:%d, y:%d, z:%d\n", camera.x, camera.y, camera.z); 
}

void Control::keyDown(unsigned char key, int x, int y)
{
	switch (key){
	case 'q':
		//�����ӵ� ����
		revolutionSpeedFactor += 0.1f;
		break;
	case 'a':
		//�����ӵ� ����
		revolutionSpeedFactor -= 0.1f;
		break;
	case 's':
		//toggle ���� �� ����
		if (doAnimation == false)
			doAnimation = true;
		else
			doAnimation = false;
		break;
	case 'w':
		//���� �� ���� ����
		doAnimation = true;
		break;
	case 'e':
		//�����ӵ� ����
		rotationSpeedFactor += 0.1f;
		break;
	case 'd':
		//�����ӵ� ����
		rotationSpeedFactor -= 0.1f;
		break;
	case 'r':
		//����
		rotationSpeedFactor = 1.0f;
		revolutionSpeedFactor = 1.0f;
		break;
	case 'x':
		//�� �ƿ�
		zoomingOut = true;
		break;
	case 'z':
		zoomingIn = true;
		break;
	case 'o':
		//�༺�˵� ���
		orbitToggle = -orbitToggle;
		break;
	case 'c':
		//���� ���
		enableCloud = -enableCloud;
		break;
	case 'p':
		camera.toggleMode();
		break;
	case ' ':
		//���� �߻�
		launchRocket();
		break;
	case 27:
		// ESC
		glutDestroyWindow(window_id);
		exit(0);
		break;
	default:
		break;
	}
}


void Control::mouse(GLint button, GLint action, GLint x, GLint y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (action == GLUT_DOWN)
		{
			isDragging = true;
			dragOriginX = x;
			dragOriginY = y;
			original3dT = camera._3dT;
			original3dW = camera._3dW;
		}
		else
		{
			isDragging = false;
			printf("px: %f, py: %f, pz:%f\n", camera.px, camera.py, camera.pz);
		}
	}
}

void Control::mouseMove(int x, int y)
{
	if (isDragging)
	{
		camera._3dW = original3dW + (x - dragOriginX);
		camera._3dT = original3dT + (dragOriginY - y);
		if (camera._3dT >= 180)
		{
			camera._3dT = 180;
		}
		if (camera._3dT <= 1)
		{
			camera._3dT = 1;
		}
		camera.recalculate3D();
	}
}

void Control::keyUp(unsigned char key, int x, int y)
{
	switch (key){
	case 'x':
		zoomingOut = false;
		break;
	case 'z':
		zoomingIn = false;
		break;

	default:
		break;
	}
}

void Control::launchRocketOnCountdown()
{
	/*controller.rocketCountdown -= 1;
	if (controller.rocketCountdown != 0)
		return;
	
	controller.rocketCountdown = 5000;
	
	launchRocket();*/
}

void Control::launchRocket()
{
	/*if (rockets.size() >= 200)
	{
		reuseRocket();
		return;
	}

	Rocket* rocket = new Rocket();
	int bufferIndex = 0;

	glBufferSubData(GL_ARRAY_BUFFER, bufferIndex, sizeof(points), points);
	bufferIndex += sizeof(points);
	glBufferSubData(GL_ARRAY_BUFFER, bufferIndex, sizeof(colors), colors);

	rockets.push_back(rocket);
	sceneObject->addChild(rocket);
	return;*/
}

/* ���ᰡ �� �� rocket�� �����Ѵ�. */
void Control::reuseRocket()
{
	/*std::list<Rocket*>::iterator it;

	for (it = rockets.begin(); it != rockets.end(); it++)
	{
		if ((*it)->isExhausted())
		{
			(*it)->relaunch();
			return;
		}
	}*/
}