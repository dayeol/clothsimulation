#include "Cloth.h"
#include "main.h"
#include "SOIL.h"
#include <ctime>

Cloth::Cloth(float _x, float _y, int _numX, int _numY)
{
	x = _x;
	y = _y;
	numX = _numX;
	numY = _numY;
	numTriangle = 0;
	texture = SOIL_load_OGL_texture
	(
		"towel2.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	
	for (int j = 0; j <= numY; j++)
	{
		for (int i = 0; i <= numX; i++)
		{
			//내분점들을 찍는다. 총 particle은 (numX + 1) * (numY + 1) 개가 된다.
			particles.push_back(vec4(
				(-y*j) / numY,
				((-x / 2)*i + (x / 2)*(numX - i)) / numX, 
				1,
				1.0));
			particleNormals.push_back(vec3(0.0, 0.0, 1.0));
			textures.push_back(vec2((float)i/numX, (float)j/numY));
		}
	}

	
	for (int j = 0; j <= numY - 1; j++)
	{
		for (int i = 0; i <= numX - 1; i++)
		{
			int offset = (numX + 1) * j;

			if ((i % 2 == 0) && (j % 2 == 0) || (i % 2 == 1) && (j % 2 == 1))
			{
				//upper
				indices.push_back(i + offset);
				indices.push_back(i + 1 + offset);
				indices.push_back(i + numX + 1 + offset);
				numTriangle++;

				//lower
				indices.push_back(i + 1 + offset);
				indices.push_back(i + numX + 1 + offset);
				indices.push_back(i + numX + 2 + offset);
				numTriangle++;
			}

			else
			{
				//upper
				indices.push_back(i + offset);
				indices.push_back(i + 1 + offset);
				indices.push_back(i + numX + 2 + offset);
				numTriangle++;

				//lower
				indices.push_back(i + offset);
				indices.push_back(i + numX + 1 + offset);
				indices.push_back(i + numX + 2 + offset);
				numTriangle++;
			}
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
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, g_normalsBuffer);

		// Process all vertices.
		glDispatchCompute(ROWS, ROWS , 1);

		// Make sure, all vertices and normals are written.
		glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, 0);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, 0);
	}
	computeShader.UnUse();
	
	program.Use();
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glUniform1i(program["TextureColor"], 0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glUniformMatrix4fv(program("ModelView"), 1, GL_TRUE, model_view);
		// Normal Input
		glBindBuffer(GL_ARRAY_BUFFER, g_normalsBuffer);
		glVertexAttribPointer(program["vNormal"], 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Draw Lines
		glBindBuffer(GL_ARRAY_BUFFER, g_verticesBuffer[currentOutput]);
		glVertexAttribPointer(program["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, 0);
		for (int i = 0; i < numTriangle; i++)
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &(indices[i * 3]));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);

		// Draw Points
		//glBindBuffer(GL_ARRAY_BUFFER, g_verticesBuffer[currentOutput]);
		//glVertexAttribPointer(program["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, 0);
		//glDrawArrays(GL_POINTS, 0, (numX + 1)*(numY + 1));
	}
	program.UnUse();

	previousInput = (previousInput + 1) % 3;
	currentInput = (currentInput + 1) % 3;
	currentOutput = (currentOutput + 1) % 3;

	model_view = mvstack.pop();
}
