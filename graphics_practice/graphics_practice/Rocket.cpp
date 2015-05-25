#include "Rocket.h"

Rocket::Rocket()
{
	totalNumberOfVertices += 13;

	relaunch();
	
	int offset = getOffset();

	int idx = 0;
	//head
	points[offset + idx++] = point4(0.0, 3.0, 0.0, 1.0);
	points[offset + idx++] = point4(-0.5, 2.0, 0.0, 1.0);
	points[offset + idx++] = point4(0.5, 2.0, 0.0, 1.0);
	//body
	points[offset + idx++] = point4(-0.5, 2.0, 0.0, 1.0);
	points[offset + idx++] = point4(0.5, 2.0, 0.0, 1.0);
	points[offset + idx++] = point4(0.5, 0.0, 0.0, 1.0);
	points[offset + idx++] = point4(-0.5, 0.0, 0.0, 1.0);
	//left wing
	points[offset + idx++] = point4(-1.5, 0.0, 0.0, 1.0);
	points[offset + idx++] = point4(-0.5, 0.0, 0.0, 1.0);
	points[offset + idx++] = point4(-0.5, 1.0, 0.0, 1.0);
	//right wing
	points[offset + idx++] = point4(1.5, 0.0, 0.0, 1.0);
	points[offset + idx++] = point4(0.5, 0.0, 0.0, 1.0);
	points[offset + idx++] = point4(0.5, 1.0, 0.0, 1.0);
	
	for (int i = 0; i < 3; i++)
	{
		colors[offset + i] = RGB(200, 200, 255);
	}

	for (int i = 3; i < 7; i++)
	{
		colors[offset + i] = RGB(101, 150, 177);
	}

	for (int i = 7; i < 13; i++)
	{
		colors[offset + i] = RGB(200, 200, 188);
	}
	
}
void Rocket::updatePosition()
{
	if (90.0 + direction <= 360.0)
	{
		x += cos((90 + direction)*M_PI/180) * speed;
		y += sin((90 + direction)*M_PI/180) * speed;
	}
	else
	{
		x += cos((direction - 270)*M_PI/180) * speed;
		y += sin((direction - 270)*M_PI/180) * speed;
	}

	if (fuel % 1000 == 0)
	{
		rightTurn = rand() % 2;
	}

	if (rightTurn)
	{
		direction -= (float)(rand() % 200)/1000;
	}
	else
	{
		direction += (float)(rand() % 200)/1000;
	}

	if (fuel > 0)
	{
		fuel -= 1;
	}
}

void Rocket::draw()
{
	updatePosition();

	mvstack.push(model_view);

	mat4 instance = Translate(x, y, 0) * RotateZ(direction) * Scale(0.015, 0.015, 0) * Translate(0, -1.5, 0);

	model_view = model_view * instance;
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

	int offset = getOffset();

	if (fuel > 0)
	{
		glDrawArrays(GL_TRIANGLE_FAN, offset, 3);
		glDrawArrays(GL_TRIANGLE_FAN, offset + 3, 4);
		glDrawArrays(GL_TRIANGLE_FAN, offset + 7, 3);
		glDrawArrays(GL_TRIANGLE_FAN, offset + 10, 3);
	}
	model_view = mvstack.pop();
}

void Rocket::relaunch()
{
	fuel = rand() % 10000 + 1000;
	rightTurn = rand() % 2;
	x = 0;// (float)rand() / RAND_MAX;
	y = 0;// (float)rand() / RAND_MAX;
	direction = rand() % 360;
	speed = (float)((rand() % 10) + 1) / 10000;
}