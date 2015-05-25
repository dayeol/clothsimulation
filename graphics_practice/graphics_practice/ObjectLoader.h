#ifndef _ABOUTOBJ_H_
#define _ABOUTOBJ_H_

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "main.h"

using namespace std;

class Face
{
private:
	vec4 vertex1, vertex2, vertex3;
	vec2 texture1, texture2, texture3;
	vec3 normal1, normal2, normal3;
public:
	void set(vector<vec4> &vertexs, vector<long> vertexNum, int offset);
	void set(vector<vec4> &vertexs, vector<vec2> &textures, vector<long> vertexNum, vector<long> textureNum, int offset);
	void set(vector<vec4> &vertexs, vector<vec2> &textures, vector<vec3> &normals, vector<long> vertexNum, vector<long> textureNum, vector<long> normalNum, int offset);
	inline vec4 getVertex1(){ return vertex1; }
	inline vec4 getVertex2(){ return vertex2; }
	inline vec4 getVertex3(){ return vertex3; }
	vec2 getTexture1(){ return texture1; }
	vec2 getTexture2(){ return texture2; }
	vec2 getTexture3(){ return texture3; }
	vec3 getNormal1(){ return normal1; }
	vec3 getNormal2(){ return normal2; }
	vec3 getNormal3(){ return normal3; }
};

class ObjectLoader
{
public:
	vector<Face> faces;
	void readObjFile(string fileName);
	GLuint readTexture(const char *file_name);
	void addVerticesList();
	void addVerticesList(vector<vec4> &vertList);
	void addTextureCoord();
	void addTextureCoord(vector<vec2> &texList);
	void addNormals();
	void addNormals(vector<vec3> &normList);
};

#endif