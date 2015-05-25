#pragma once
#include "ObjectLoader.h"

using namespace std;

class Face
{
private:
	vec3 vertex1, vertex2, vertex3;
	vec2 texture1, texture2, texture3;
	vec3 normal1, normal2, normal3;
	vector<vec3> vertexList;

public:
	void set(vector<vec3> &vertexs, vector<long> vertexNum, int offset);
	void set(vector<vec3> &vertexs, vector<vec2> &textures, vector<long> vertexNum, vector<long> textureNum, int offset);
	void set(vector<vec3> &vertexs, vector<vec2> &textures, vector<vec3> &normals, vector<long> vertexNum, vector<long> textureNum, vector<long> normalNum, int offset);
	vec3 getVertex1(){ return vertex1; }
	vec3 getVertex2(){ return vertex2; }
	vec3 getVertex3(){ return vertex3; }
	vec2 getTexture1(){ return texture1; }
	vec2 getTexture2(){ return texture2; }
	vec2 getTexture3(){ return texture3; }
	vec3 getNormal1(){ return normal1; }
	vec3 getNormal2(){ return normal2; }
	vec3 getNormal3(){ return normal3; }
	vector<vec3> getVertexList(){ return vertexList; }
};