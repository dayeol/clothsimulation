#include "Planet.h"
#include "Control.h"
#include "Camera.h"
mat4 colorFilter = mat4(0.5, 0, 0, 0,
	0, 0.5, 0, 0,
	0, 0, 0.5, 0,
	0, 0, 0, 1.0);

unsigned int Planet::numberOfPlanets = 0;
unsigned int Planet::planetVertices = 0;
unsigned int Planet::orbitVertices = 0;

Planet::Planet(vec4 _color, float _size, float _distance, float revolutionSpeed, float rotationSpeed, char* textureName)
{
	planetID = numberOfPlanets;
	cout << planetID << endl;
	size = _size;
	distance = _distance;
	revSpeed = revolutionSpeed;
	rotSpeed = rotationSpeed;
	color = color;

	revAngle = 0;
	rotAngle = 0;
	
	int n = 0;
	
	texture = SOIL_load_OGL_texture
		(
		textureName,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

	numberOfPlanets += 1;

	for (int i = 0; i < planetVertices + orbitVertices; i++)
		colors.push_back(_color);

	totalNumberOfVertices += (planetVertices + orbitVertices);

	//cout << getOffset() << endl;
	cout << "Planet making is done!" << endl;
}

Planet::Planet()
{
}

Planet::~Planet()
{
}

void Planet::updateAngle()
{
	revAngle += controller.doAnimation ? revSpeed * controller.animationSpeed * controller.revolutionSpeedFactor : 0;
	rotAngle += controller.doAnimation ? rotSpeed * controller.animationSpeed * controller.rotationSpeedFactor : 0;

	if (revAngle >= 360.0f)
		revAngle -= 360.0f;
	
	if (rotAngle >= 360.0f)
		rotAngle -= 360.0f;
}

void Planet::draw(bool isSelect)
{	
	if (!isSelect)
	{
		glUniform1i(IsEarth, 0); // This planet is not Earth
	}
	updateAngle();
	int offset = getOffset();

	// Orbit 
	mvstack.push(model_view);
	if (orbitToggle > 0)
	{
		if (!isSelect)
		{
			glUniform1i(IsShading, 0);
			glUniform1i(UseTexture, 0);
		}
		glUniformMatrix4fv(isSelect ? sModelView : ModelView, 1, GL_TRUE, model_view * Scale(distance));

		for (int i = 0; i < orbitVertices / 3; i++)
			glDrawArrays(GL_TRIANGLES, planetVertices + offset + i * 3, 3);
	}
	model_view = mvstack.pop();
	
	GLfloat Tx = distance * cos(revAngle * M_PI / 180);
	GLfloat Ty = distance * sin(revAngle * M_PI / 180);

	//if this is selected planet
	if (selectedPlanet == id && id != 0)
	{
		camera.x = 0.5*Tx;
		camera.y = 0.5*-Ty;
		camera._3dx = 0.5*Tx;
		camera._3dy = 0.5*-Ty;
		camera._3dz = 0;
		camera.recalculate3D();
		
		mvstack.push(model_view);
		if (orbitToggle > 0)
		{
			model_view = model_view * Translate(Tx, 0, Ty) * Scale(size / 2 * 1.1);
			if (!isSelect)
			{
				glUniform1i(IsShading, 0);
			}
			glUniformMatrix4fv(isSelect ? sModelView : ModelView, 1, GL_TRUE, model_view);
			glDrawArrays(GL_LINE_STRIP, totalNumberOfVertices, 36);
		}
		model_view = mvstack.pop();
	}

	mat4 instance = Translate(Tx, 0, Ty) * RotateY(rotAngle) * Scale(size / 2);

	model_view = model_view * instance;

	mvstack.push(model_view);
	if (planetID == 0)
	{
		if (!isSelect)
			glUniform1i(IsShading, 0);
	}
	else
	{
		if (!isSelect)
			glUniform1i(IsShading, 1);
	}

	if (!isSelect)
	{
		glUniform1i(UseTexture, 1);
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		int textureColor = glGetUniformLocation(program, "TextureColor");
		glUniform1i(textureColor, 0);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	glUniformMatrix4fv(isSelect ? sModelView : ModelView, 1, GL_TRUE, model_view);
	
	for (int i = 0; i < planetVertices / 3; i++)
	{
		glDrawArrays(GL_TRIANGLES, offset + i * 3, 3);
	}

	if (!isSelect)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
	model_view = mvstack.pop();
}
