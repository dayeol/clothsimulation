#include "Floor.h"
#include "Control.h"
#include "Camera.h"
#include "SOIL.h"

Floor::Floor(float _x, float _y)
{
	x = _x;
	y = _y;
	size = 0;

	texture = SOIL_load_OGL_texture
	(
		"grass.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	for (int i = -x; i <= x; i++)
	{
		for (int j = -y; j <= y; j++)
		{
			float left = i + x;
			float right = i + 1 + x;
			float up = j + 1 + y;
			float down = j + y;
			
			vertices.push_back(vec4(i - 0.5, j  - 0.5, -1, 1));
			vertices.push_back(vec4(i - 0.5, j  + 0.5, -1, 1));
			vertices.push_back(vec4(i + 0.5, j - 0.5, -1, 1));

			vertices.push_back(vec4(i - 0.5, j + 0.5, -1, 1));
			vertices.push_back(vec4(i + 0.5, j - 0.5, -1, 1));
			vertices.push_back(vec4(i  + 0.5, j  + 0.5, -1, 1));
			
			normals.push_back(vec3(0, 0, 1));
			normals.push_back(vec3(0, 0, 1));
			normals.push_back(vec3(0, 0, 1));
			normals.push_back(vec3(0, 0, 1));
			normals.push_back(vec3(0, 0, 1));
			normals.push_back(vec3(0, 0, 1));

			textures.push_back(vec2(left / (float)(x * 2 + 1), down / (float)(y * 2 + 1)));
			textures.push_back(vec2(left / (float)(x * 2 + 1), up / (float)(y * 2 + 1)));
			textures.push_back(vec2(right / (float)(x * 2 + 1), down / (float)(y * 2 + 1)));

			textures.push_back(vec2(left / (float)(x * 2 + 1), up / (float)(y * 2 + 1)));
			textures.push_back(vec2(right / (float)(x * 2 + 1), down / (float)(y * 2 + 1)));
			textures.push_back(vec2(right / (float)(x * 2 + 1), up / (float)(y * 2 + 1)));

			totalNumberOfVertices+= 6;
			size+= 2;
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
		glUniform1i(objectShader("isWireframe"), controller.isWireframe);
		glUniform1i(objectShader("isFloor"), 1);
		glUniform1i(objectShader("hasNormal"), 0);
		glUniform1i(objectShader("isSpecular"), 0);

		glActiveTexture(GL_TEXTURE4);
		glEnable(GL_TEXTURE_2D);
		glUniform1i(objectShader("TextureColor"), 4);
		glBindTexture(GL_TEXTURE_2D, texture);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		unsigned int bufferIndex = 0;
		glVertexAttribPointer(objectShader["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));
		bufferIndex += sizeof(vec4) * vertices.size();

		glVertexAttribPointer(objectShader["vTexCoord"], 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));
		bufferIndex += sizeof(vec2) * textures.size();

		glVertexAttribPointer(objectShader["v_normal"], 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));
		bufferIndex += sizeof(vec3) * normals.size();
		
		glUniformMatrix4fv(objectShader("ModelView"), 1, GL_TRUE, model_view * Scale(0.5, 0.5, 1));
		
		for (int i = 0; i < size; i++)
			glDrawArrays(controller.isWireframe?GL_LINE_LOOP:GL_TRIANGLES, i * 3, 3);

		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
	objectShader.UnUse();

	model_view = mvstack.pop();
}

void Floor::reset()
{
}