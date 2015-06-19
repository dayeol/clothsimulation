#include "Bar.h"
#include "Control.h"
#include "SOIL.h"

Bar::Bar()
{
	offset = totalNumberOfVertices;
	
	texture = SOIL_load_OGL_texture
	(
		"wood.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	numVertex = 0;

	float width = 1.15;
	float thick = 0.05;
	float height = 1.05;

	// Horizontal Upper rect
	vertices.push_back(vec4(thick, -width, height + thick, 1));
	vertices.push_back(vec4(-thick, -width, height + thick, 1));
	vertices.push_back(vec4(thick, width, height + thick, 1));

	vertices.push_back(vec4(-thick, -width, height + thick, 1));
	vertices.push_back(vec4(thick, width, height + thick, 1));
	vertices.push_back(vec4(-thick, width, height + thick, 1));

	for (int i = 0; i < 6; i++)
	{
		normals.push_back(vec3(0, 0, -1));
		numVertex += 1;
	}

	// Horizontal Behind rect
	vertices.push_back(vec4(thick, -width, height + thick, 1));
	vertices.push_back(vec4(thick, width, height + thick, 1));
	vertices.push_back(vec4(thick, -width, height - thick, 1));

	vertices.push_back(vec4(thick, width, height + thick, 1));
	vertices.push_back(vec4(thick, -width, height - thick, 1));
	vertices.push_back(vec4(thick, width, height - thick, 1));

	for (int i = 0; i < 6; i++)
	{
		normals.push_back(vec3(-1, 0, 0));
		numVertex += 1;
	}

	// Horizontal Front rect
	vertices.push_back(vec4(-thick, -width, height + thick, 1));
	vertices.push_back(vec4(-thick, width, height + thick, 1));
	vertices.push_back(vec4(-thick, -width, height - thick, 1));

	vertices.push_back(vec4(-thick, width, height + thick, 1));
	vertices.push_back(vec4(-thick, -width, height - thick, 1));
	vertices.push_back(vec4(-thick, width, height - thick, 1));

	for (int i = 0; i < 6; i++)
	{
		normals.push_back(vec3(1, 0, 0));
		numVertex += 1;
	}

	// Horizontal Lower rect
	vertices.push_back(vec4(thick, -width, height - thick, 1));
	vertices.push_back(vec4(-thick, -width, height - thick, 1));
	vertices.push_back(vec4(thick, width, height - thick, 1));

	vertices.push_back(vec4(-thick, -width, height - thick, 1));
	vertices.push_back(vec4(thick, width, height - thick, 1));
	vertices.push_back(vec4(-thick, width, height - thick, 1));

	for (int i = 0; i < 6; i++)
	{
		normals.push_back(vec3(0, 0, 1));
		numVertex += 1;
	}

	// Horizontal Left rect
	vertices.push_back(vec4(thick, -width, height + thick, 1));
	vertices.push_back(vec4(-thick, -width, height + thick, 1));
	vertices.push_back(vec4(thick, -width, height - thick, 1));

	vertices.push_back(vec4(-thick, -width, height + thick, 1));
	vertices.push_back(vec4(thick, -width, height - thick, 1));
	vertices.push_back(vec4(-thick, -width, height - thick, 1));

	for (int i = 0; i < 6; i++)
	{
		normals.push_back(vec3(0, 1, 0));
		numVertex += 1;
	}

	// Horizontal Right rect
	vertices.push_back(vec4(thick, width, height + thick, 1));
	vertices.push_back(vec4(-thick, width, height + thick, 1));
	vertices.push_back(vec4(thick, width, height - thick, 1));

	vertices.push_back(vec4(-thick, width, height + thick, 1));
	vertices.push_back(vec4(thick, width, height - thick, 1));
	vertices.push_back(vec4(-thick, width, height - thick, 1));

	for (int i = 0; i < 6; i++)
	{
		normals.push_back(vec3(0, -1, 0));
		numVertex += 1;
	}

	// Vertical Behind rect
	vertices.push_back(vec4(thick, width - 2 * thick, height - thick, 1));
	vertices.push_back(vec4(thick, width, height - thick, 1));
	vertices.push_back(vec4(thick, width - 2 * thick, -1, 1));

	vertices.push_back(vec4(thick, width, height - thick, 1));
	vertices.push_back(vec4(thick, width - 2 * thick, -1, 1));
	vertices.push_back(vec4(thick, width, -1, 1));

	for (int i = 0; i < 6; i++)
	{
		normals.push_back(vec3(-1, 0, 0));
		numVertex += 1;
	}

	// Vertical Front rect
	vertices.push_back(vec4(-thick, width - 2 * thick, height - thick, 1));
	vertices.push_back(vec4(-thick, width, height - thick, 1));
	vertices.push_back(vec4(-thick, width - 2 * thick, -1, 1));

	vertices.push_back(vec4(-thick, width, height - thick, 1));
	vertices.push_back(vec4(-thick, width - 2 * thick, -1, 1));
	vertices.push_back(vec4(-thick, width, -1, 1));

	for (int i = 0; i < 6; i++)
	{
		normals.push_back(vec3(1, 0, 0));
		numVertex += 1;
	}

	// Vertical Left rect
	vertices.push_back(vec4(thick, width - 2 * thick, height - thick, 1));
	vertices.push_back(vec4(-thick, width - 2 * thick, height - thick, 1));
	vertices.push_back(vec4(-thick, width - 2 * thick, -1, 1));

	vertices.push_back(vec4(thick, width - 2 * thick, height - thick, 1));
	vertices.push_back(vec4(-thick, width - 2 * thick, -1, 1));
	vertices.push_back(vec4(thick, width - 2 * thick, -1, 1));

	for (int i = 0; i < 6; i++)
	{
		normals.push_back(vec3(0, 1, 0));
		numVertex += 1;
	}

	// Vertical Right rect
	vertices.push_back(vec4(thick, width, height - thick, 1));
	vertices.push_back(vec4(-thick, width, height - thick, 1));
	vertices.push_back(vec4(-thick, width, -1, 1));

	vertices.push_back(vec4(thick, width, height - thick, 1));
	vertices.push_back(vec4(-thick, width, -1, 1));
	vertices.push_back(vec4(thick, width, -1, 1));

	for (int i = 0; i < 6; i++)
	{
		normals.push_back(vec3(0, -1, 0));
		numVertex += 1;
	}

	for (int i = 0; i < numVertex / 6; i++)
	{
		textures.push_back(vec2(0, 0));
		textures.push_back(vec2(0, 1));
		textures.push_back(vec2(1, 0));
		textures.push_back(vec2(0, 1));
		textures.push_back(vec2(1, 0));
		textures.push_back(vec2(1, 1));
	}

	totalNumberOfVertices += numVertex;
}

Bar::~Bar()
{
}

void Bar::draw()
{
	mvstack.push(model_view);

	objectShader.Use();
	{
		glUniform1i(objectShader("isWireframe"), controller.isWireframe);
		glUniform1i(objectShader("isFloor"), 0);

		glActiveTexture(GL_TEXTURE6);
		glEnable(GL_TEXTURE_2D);
		glUniform1i(program("TextureColor"), 6);
		glBindTexture(GL_TEXTURE_2D, texture);

		glUniformMatrix4fv(objectShader("ModelView"), 1, GL_TRUE, model_view * Translate(1, 0, 0));

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		unsigned int bufferIndex = 0;
		glVertexAttribPointer(objectShader["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));
		bufferIndex += sizeof(vec4) * vertices.size();

		glVertexAttribPointer(objectShader["vTexCoord"], 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));
		bufferIndex += sizeof(vec2) * textures.size();

		glVertexAttribPointer(objectShader["v_normal"], 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));
		bufferIndex += sizeof(vec3) * normals.size();

		for (int i = 0; i < numVertex / 3; i++)
			glDrawArrays(controller.isWireframe ? GL_LINE_LOOP : GL_TRIANGLES, offset + i * 3, 3);

		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
	objectShader.UnUse();

	model_view = mvstack.pop();
}

void Bar::reset()
{
	
}