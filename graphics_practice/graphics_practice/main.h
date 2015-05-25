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

using namespace std;

#define RGB(r,g,b)	color4( (float)r/255, (float)g/255, (float)b/255, 1.0 )
#define RGBA(r,g,b,a)	color4( (float)r/255, (float)g/255, (float)b/255, (float)a/255 )
#define BUFFER_OFFSET(bytes) ((GLvoid*) (bytes))

typedef vec4 point4;
typedef vec4 color4;

extern class Object* sceneObject;

extern GLuint program, selectProgram;
extern GLuint earthProgram;

extern MatrixStack mvstack;
extern mat4 model_view, look_at;
extern GLuint ModelView, Projection, LookAtMat;
extern GLuint sModelView, sProjection;
extern GLuint eModelView, eProjection, eLookAtMat;

extern GLuint sCode;
extern GLuint textureLocation, textureLocationNight, textureNormalMap, textureSpec, textureCloud, EarthTime;

extern GLuint IsShading;
extern GLuint IsEarth;
extern GLuint UseTexture;
extern GLuint EnableCloud;

extern vector<vec4> vertices; // vertex list
extern vector<vec4> colors;
extern vector<vec3> normals;
extern vector<vec2> textures;

extern int orbitToggle; // orbit 보여주기 유무
extern int enableCloud; // 구름 보여주기 유무

extern int selectedPlanet;