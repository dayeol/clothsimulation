#pragma once

#include "Angel.h"
#include "vec.h"
#include "mat.h"
#include <math.h>
#include "Object.h"
#include "ObjectLoader.h"
#include "MatrixStack.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "GLSLShader.h"

using namespace std;

#define ROWS	20
#define COLS	20

#define RGB(r,g,b)	color4( (float)r/255, (float)g/255, (float)b/255, 1.0 )
#define RGBA(r,g,b,a)	color4( (float)r/255, (float)g/255, (float)b/255, (float)a/255 )
#define BUFFER_OFFSET(bytes) ((GLvoid*) (bytes))

extern class Object* sceneObject;

extern GLSLShader program, computeShader, computeShadow, objectShader, skyShader, shadowShader;

extern MatrixStack mvstack;
extern mat4 model_view, look_at;

extern vector<vec4> vertices; // vertex list
extern vector<vec4> particles;
extern vector<vec4> shadows;
extern vector<vec4> particleNormals;
extern vector<vec4> colors;
extern vector<vec3> normals;
extern vector<vec2> textures;
extern vector<vec2> particleTextures;
extern vector<GLushort> indices;

extern GLuint g_verticesBuffer[3];
extern GLuint g_shadowBuffer;
extern GLuint g_normalsBuffer;
extern GLuint g_objectVerticesBuffer;
extern GLuint vertexBuffer;
extern GLuint vertexBufferCloth;
extern GLuint shadowTexture;