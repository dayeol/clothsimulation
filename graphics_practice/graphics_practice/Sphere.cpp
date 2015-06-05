#include "Sphere.h"
#include "Control.h"
#include "SOIL.h"
int Sphere::sphereOffset = 0;

Sphere::Sphere(int id, int _numVertex)
{
	cout << "a" << endl;
	offset = totalNumberOfVertices;
	sphere_id = id;
	numVertex = _numVertex;

	texture = SOIL_load_OGL_texture
	(
		"towel.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	if (sphere_id == 0)
	{
		sphereOffset = offset;
	}

	for (int i = 0; i < numVertex; i++)
		colors.push_back(vec4(1, 0, 1, 1));

	totalNumberOfVertices += numVertex;
}

Sphere::~Sphere()
{
}

void Sphere::draw()
{
	mvstack.push(model_view);

	objectShader.Use();
	{
		glUniform1i(objectShader["isFloor"], 1);

		/*glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glUniform1i(objectShader["textureColor"], 0);
		glBindTexture(GL_TEXTURE_2D, texture);*/
		
		model_view = model_view * Translate(controller.sphereX, 0, 0) * Scale(0.5);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(objectShader["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, 0);
		unsigned int idx = sizeof(vec4) * vertices.size();
		//glVertexAttribPointer(objectShader["vTexCoord"], 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(idx));
		
		glUniformMatrix4fv(objectShader("ModelView"), 1, GL_TRUE, model_view);
		for (int i = 0; i < numVertex / 3; i++)
			glDrawArrays(GL_TRIANGLES, offset + i * 3, 3);
		
		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);*/
	}
	objectShader.UnUse();

	model_view = mvstack.pop();
}