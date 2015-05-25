#include "Face.h"

void Face::set(vector<vec3> &vertexs, vector<long> vertexNum, int offset)
{
	vertex1 = vertexs[vertexNum[0] - 1];
	vertex2 = vertexs[vertexNum[offset] - 1];
	vertex3 = vertexs[vertexNum[offset + 1] - 1];
	for (int i = 0; i < offset; i++)
		vertexList.push_back(vertexs[vertexNum[i] - 1]);
}

void Face::set(vector<vec3> &vertexs, vector<vec2> &textures, vector<long> vertexNum, vector<long> textureNum, int offset)
{
	vertex1 = vertexs[vertexNum[0] - 1];
	vertex2 = vertexs[vertexNum[offset] - 1];
	vertex3 = vertexs[vertexNum[offset + 1] - 1];
	for (int i = 0; i < offset; i++)
		vertexList.push_back(vertexs[vertexNum[i] - 1]);

	// texture is not used
	/*
	texture1 = textures[textureNum[0] - 1];
	texture2 = textures[textureNum[1] - 1];
	texture3 = textures[textureNum[2] - 1];
	*/
}

void Face::set(vector<vec3> &vertexs, vector<vec2> &textures, vector<vec3> &normals, vector<long> vertexNum, vector<long> textureNum, vector<long> normalNum, int offset)
{
	vertex1 = vertexs[vertexNum[0] - 1];
	vertex2 = vertexs[vertexNum[offset] - 1];
	vertex3 = vertexs[vertexNum[offset + 1] - 1];
	for (int i = 0; i < offset; i++)
		vertexList.push_back(vertexs[vertexNum[i] - 1]);

	// texture is not used
	/*
	texture1 = textures[textureNum[0] - 1];
	texture2 = textures[textureNum[1] - 1];
	texture3 = textures[textureNum[2] - 1];
	*/

	normal1 = normals[normalNum[0] - 1];
	normal2 = normals[normalNum[offset] - 1];
	normal3 = normals[normalNum[offset + 1] - 1];
}