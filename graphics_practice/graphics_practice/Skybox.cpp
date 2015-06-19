#include "Skybox.h"
#include "Control.h"
#include "SOIL.h"

Skybox::Skybox()
{
	offset = totalNumberOfVertices;

	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};
	
	numVertex = 0;

	for (int i = 0; i < 36; i++)
	{
		vertices.push_back(vec4(10 * skyboxVertices[i * 3], 10 * skyboxVertices[i * 3 + 1], 10 * skyboxVertices[i * 3 + 2], 1));
		normals.push_back(vec3(0, 0, 0));
		textures.push_back(vec2(0, 0));
		numVertex += 1;
	}

	vector<const GLchar*> faces;
	faces.push_back("skybox/right.jpg");
	faces.push_back("skybox/left.jpg");
	faces.push_back("skybox/top.jpg");
	faces.push_back("skybox/bottom.jpg");
	faces.push_back("skybox/back.jpg");
	faces.push_back("skybox/front.jpg");

	texture = loadCubemap(faces);

	/*texture = SOIL_load_OGL_cubemap
		(
		"skybox/right.tga",
		"skybox/left.tga",
		"skybox/front.tga",
		"skybox/back.tga",
		"skybox/top.tga",
		"skybox/bottom.tga",
		SOIL_LOAD_L,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
		);*/
	
	totalNumberOfVertices += numVertex;
}

Skybox::~Skybox()
{
}

void Skybox::draw()
{
	mvstack.push(model_view);

	if (controller.isWireframe == false)
	{
		skyShader.Use();
		{
			glActiveTexture(GL_TEXTURE7);
			glEnable(GL_TEXTURE_CUBE_MAP);
			glUniform1i(skyShader("Skybox"), 7);
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

			glUniformMatrix4fv(skyShader("ModelView"), 1, GL_TRUE, model_view * Translate(0, 0, 0) * RotateX(90));

			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			unsigned int bufferIndex = 0;
			glVertexAttribPointer(skyShader["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bufferIndex));

			glDrawArrays(GL_TRIANGLES, offset, 36);

			glActiveTexture(GL_TEXTURE7);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glDisable(GL_TEXTURE_CUBE_MAP);
		}
		skyShader.UnUse();
	}
	model_view = mvstack.pop();
}

void Skybox::reset()
{

}

GLuint Skybox::loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}