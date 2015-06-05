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

			colors.push_back(vec4(1, 0.5, 1, 1));
			colors.push_back(vec4(1, 0.5, 1, 1));
			colors.push_back(vec4(1, 0.5, 1, 1));
			colors.push_back(vec4(1, 0.5, 1, 1));

			normals.push_back(vec3(0, 0, 1));
			normals.push_back(vec3(0, 0, 1));
			normals.push_back(vec3(0, 0, 1));
			normals.push_back(vec3(0, 0, 1));

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

	floorShader.Use();
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(floorShader["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, 0);
		glUniformMatrix4fv(floorShader("ModelView"), 1, GL_TRUE, model_view);
		for (int i = 0; i < size; i++)
			glDrawArrays(GL_LINE_LOOP, i * 4, 4);
	}
	floorShader.UnUse();

	model_view = mvstack.pop();
}
