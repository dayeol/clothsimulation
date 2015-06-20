#include "Sphere.h"
#include "Control.h"
#include "Cloth.h"
#include "SOIL.h"
int Sphere::sphereOffset = 0;

Sphere::Sphere(int id, int _numVertex)
{
	offset = totalNumberOfVertices;
	sphere_id = id;
	numVertex = _numVertex;

	texture[0] = SOIL_load_OGL_texture
	(
		"ball.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	texture[1] = SOIL_load_OGL_texture
		(
		"ball_normal.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

	if (sphere_id == 0)
	{
		sphereOffset = offset;
	}

	for (int i = 0; i < numVertex; i++)
	{
		vec4 p = vertices[i + offset];
		shadows.push_back(vec4(p.x, p.y, -0.98, 1));
	}

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
		glUniform1i(objectShader("isWireframe"), controller.isWireframe);
		glUniform1i(objectShader("isFloor"), 0);
		glUniform1i(objectShader("hasNormal"), 1);
		glUniform1i(objectShader("isSpecular"), 1);

		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glUniform1i(objectShader("TextureColor"), 2);
		glBindTexture(GL_TEXTURE_2D, texture[0]);

		glActiveTexture(GL_TEXTURE3);
		glEnable(GL_TEXTURE_2D);
		glUniform1i(objectShader("TextureNormal"), 3);
		glBindTexture(GL_TEXTURE_2D, texture[1]);

		glUniformMatrix4fv(objectShader("ModelView"), 1, GL_TRUE, model_view * Translate(controller.sphereX, 0, 0) * Scale(0.5));

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		unsigned int bufferIndex = 0;
		glVertexAttribPointer(objectShader["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));
		bufferIndex += sizeof(vec4) * vertices.size();
		
		glVertexAttribPointer(objectShader["vTexCoord"], 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));
		bufferIndex += sizeof(vec2) * textures.size();
		
		glVertexAttribPointer(objectShader["v_normal"], 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));
		bufferIndex += sizeof(vec3) * normals.size();
		
		for (int i = 0; i < numVertex / 3; i++)
			glDrawArrays(controller.isWireframe ? GL_LINE_LOOP:GL_TRIANGLES, offset + i * 3, 3);
		
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
	objectShader.UnUse();
	/*
	computeShadow.Use();
	{
		glUniform1f(computeShadow("row"), ROWS);
		glUniform1i(computeShadow("col"), COLS);
		
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, g_objectVerticesBuffer);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, g_shadowBuffer);

		// Process all vertices.
		glDispatchCompute(1, 1, 1);

		// Make sure, all vertices and normals are written.
		glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);
	}
	computeShadow.UnUse();
	
	shadowShader.Use();
	{
		glActiveTexture(GL_TEXTURE10);
		glEnable(GL_TEXTURE_2D);
		glUniform1i(shadowShader("TextureColor"), 10);
		glBindTexture(GL_TEXTURE_2D, shadowTexture);

		glBindBuffer(GL_ARRAY_BUFFER, g_shadowBuffer);
		glVertexAttribPointer(shadowShader["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(shadowShader["vPosition"]);

		glUniformMatrix4fv(shadowShader("ModelView"), 1, GL_TRUE, model_view * Translate(controller.sphereX, 0, 0) * Scale(0.5));

		// Draw Lines 
		glBindBuffer(GL_ARRAY_BUFFER, g_shadowBuffer);
		glVertexAttribPointer(shadowShader["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, 0);

		if (controller.isWireframe == false)
		{
			for (int i = 0; i < numVertex / 3; i++)
				glDrawArrays(GL_TRIANGLES, (ROWS + 1) * (COLS + 1) + offset + i * 3, 3);
		}

		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
	shadowShader.UnUse();
	*/
	model_view = mvstack.pop();
}

void Sphere::reset()
{
	controller.sphereX = 0;
}