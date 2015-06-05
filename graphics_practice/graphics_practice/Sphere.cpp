#include "Sphere.h"
#include "Control.h"
int Sphere::sphereOffset = 0;

Sphere::Sphere(int id, int _numVertex)
{
	cout << "a" << endl;
	offset = totalNumberOfVertices;
	sphere_id = id;
	numVertex = _numVertex;

	if (sphere_id == 0)
	{
		sphereOffset = offset;
	}

	for (int i = 0; i < numVertex; i++)
		colors.push_back(vec4(1, 1, 1, 1));

	totalNumberOfVertices += numVertex;
}

Sphere::~Sphere()
{
}

void Sphere::draw()
{
	mvstack.push(model_view);

	sphereShader.Use();
	{
		model_view = model_view * Translate(controller.sphereX, 0, 0) * Scale(0.5);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(sphereShader["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, 0);
		glUniformMatrix4fv(sphereShader("ModelView"), 1, GL_TRUE, model_view);
		for (int i = 0; i < numVertex / 3; i++)
			glDrawArrays(GL_TRIANGLES, offset + i * 3, 3);
	}
	sphereShader.UnUse();

	model_view = mvstack.pop();
}