#include"Earth.h"
#include "Control.h"
#include "Camera.h"

Earth::Earth(vec4 _color, float _size, float _distance, float revolutionSpeed, float rotationSpeed)
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

	char* textureFile1 = "planet_textures/Earth/ColorMap_Day.jpg";
	char* textureFile2 = "planet_textures/Earth/ColorMap_Night.jpg";
	char* textureFile3 = "planet_textures/Earth/NormalMap.png";
	char* textureFile4 = "planet_textures/Earth/SpecMask.png";
	char* textureFile5 = "planet_textures/Earth/CloudMap.png";
	
	tex[0] = addEarthTexture(textureFile1);
	tex[1] = addEarthTexture(textureFile2);
	tex[2] = addEarthTexture(textureFile3);
	tex[3] = addEarthTexture(textureFile4);
	tex[4] = addEarthTexture(textureFile5);

	numberOfPlanets += 1;

	for (int i = 0; i < planetVertices + orbitVertices; i++)
		colors.push_back(_color);

	totalNumberOfVertices += (planetVertices + orbitVertices);

	cout << "Planet making is done!" << endl;
}


Earth::~Earth()
{
}


void Earth::draw(bool isSelect)
{
	if (!isSelect)
	{
		glUniform1i(IsEarth, 1);	// set this planet is earth
		//glUniform1i(EnableCloud, enableCloud);
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
				glUniform1i(IsShading, 0);
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
		{
			glUniform1i(IsShading, 0);
		}
	}
	else
	{
		if (!isSelect)
		{
			glUniform1i(IsShading, 1);
		}
	}

	if (enableCloud > 0)
	{
		glUniform1i(EnableCloud, 1);
	}
	else
	{
		glUniform1i(EnableCloud, 0);
	}


	if (!isSelect)
	{
		glUniform1i(UseTexture, 1);

		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		int textureColor = glGetUniformLocation(program, "TextureColor");
		glUniform1i(textureColor, 0);
		glBindTexture(GL_TEXTURE_2D, tex[0]);

		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		int textureColorNight = glGetUniformLocation(program, "TextureColorNight");
		glUniform1i(textureColorNight, 1);
		glBindTexture(GL_TEXTURE_2D, tex[1]);

		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		int textureNormal = glGetUniformLocation(program, "TextureNormalMap");
		glUniform1i(textureNormal, 2);
		glBindTexture(GL_TEXTURE_2D, tex[2]);

		glActiveTexture(GL_TEXTURE3);
		glEnable(GL_TEXTURE_2D);
		int textureSpec = glGetUniformLocation(program, "TextureSpec");
		glUniform1i(textureSpec, 3);
		glBindTexture(GL_TEXTURE_2D, tex[3]);

		glActiveTexture(GL_TEXTURE4);
		glEnable(GL_TEXTURE_2D);
		int textureCloud = glGetUniformLocation(program, "TextureCloud");
		glUniform1i(textureCloud, 4);
		glBindTexture(GL_TEXTURE_2D, tex[4]);
	}
	glUniformMatrix4fv(isSelect ? sModelView : ModelView, 1, GL_TRUE, model_view);

	for (int i = 0; i < planetVertices / 3; i++)
	{
		glDrawArrays(GL_TRIANGLES, offset + i * 3, 3);
	}
	if (!isSelect)
	{
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
	model_view = mvstack.pop();

	if (!isSelect)
	{
		glUniform1i(IsEarth, 0);	// set next planets are not earth


	}
}

GLuint Earth::addEarthTexture(char* textureFile)
{
	return SOIL_load_OGL_texture
		(
		textureFile,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
}