#include "Floor.h"
#include "Control.h"
#include "Camera.h"

Floor::Floor(float _x, float _y)
{
	x = _x;
	y = _y;
	size = 0;

	for (int i = -x; i <= x; i++)
	{
		for (int j = -y; j <= y; j++)
		{
			vertices.push_back(vec4(i  - 0.5, j  - 0.5, -1, 1));
			vertices.push_back(vec4(i  - 0.5, j  + 0.5, -1, 1));
			vertices.push_back(vec4(i  + 0.5, j  + 0.5, -1, 1));
			vertices.push_back(vec4(i  + 0.5, j  - 0.5, -1, 1));

			normals.push_back(vec3(0, 0, 1));
			normals.push_back(vec3(0, 0, 1));
			normals.push_back(vec3(0, 0, 1));
			normals.push_back(vec3(0, 0, 1));

			textures.push_back(vec2(0, 0));
			textures.push_back(vec2(0, 0));
			textures.push_back(vec2(0, 0));
			textures.push_back(vec2(0, 0));

			totalNumberOfVertices+= 4;
			size++;
		}
	}
}

Floor::Floor()
{
}

Floor::~Floor()
{
}

void Floor::draw()
{
	mvstack.push(model_view);

	objectShader.Use();
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(objectShader["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, 0);
		glUniformMatrix4fv(objectShader("ModelView"), 1, GL_TRUE, model_view);
		glUniform1i(objectShader["isFloor"], 0);
		for (int i = 0; i < size; i++)
			glDrawArrays(GL_LINE_LOOP, i * 4, 4);
	}
	objectShader.UnUse();

	model_view = mvstack.pop();
}
