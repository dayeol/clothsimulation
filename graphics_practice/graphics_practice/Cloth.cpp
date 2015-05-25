#include "Cloth.h"


Cloth::Cloth(float _x, float _y, int _numX, int _numY)
{
	x = _x;
	y = _y;
	numX = _numX;
	numY = _numY;
	for (int j = 0; j <= numY; j++)
	{
		for (int i = 0; i <= numX; i++)
		{
			//내분점들을 찍는다. 총 particle은 (numX + 1) * (numY + 1) 개가 된다.
			particles.push_back(vec4(
				((-y / 2)*j + (y / 2)*(numY - j)) / numY,
				((-x / 2)*i + (x / 2)*(numX - i)) / numX, 
				1,
				1.0));
		}
	}
}

void Cloth::draw()
{
	static GLint previousInput = 0;
	static GLint currentInput = 1;
	static GLint currentOutput = 2;

	mvstack.push(model_view);

	computeShader.Use();
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, g_verticesBuffer[previousInput]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, g_verticesBuffer[currentInput]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, g_verticesBuffer[currentOutput]);

		// Process all vertices.
		glDispatchCompute(1, 1, 1);

		// Make sure, all vertices and normals are written.
		glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, 0);
	}
	computeShader.UnUse();


	program.Use();
	{
		glBindBuffer(GL_ARRAY_BUFFER, g_verticesBuffer[currentOutput]);
		glVertexAttribPointer(program["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_POINTS, 0, (numX + 1)*(numY + 1));
		glUniformMatrix4fv(program("ModelView"), 1, GL_TRUE, model_view);
	}
	program.UnUse();

	previousInput = (previousInput + 1) % 3;
	currentInput = (currentInput + 1) % 3;
	currentOutput = (currentOutput + 1) % 3;

	model_view = mvstack.pop();
}
