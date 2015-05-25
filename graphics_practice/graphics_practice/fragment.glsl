#version 440
// per-fragment interpolated values from the vertex shader
in  vec3 fN;
in  vec3 fL;
in  vec3 fE;
in  vec2 texCoord;
in  vec2 cTexCoord;
in	vec3 fTangent;
in	vec3 fBitangent;

in  vec4 color;
out vec4 fColor;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform float Shininess;
uniform int IsShading;
uniform sampler2D TextureColor;
uniform sampler2D TextureColorNight;
uniform sampler2D TextureNormalMap;
uniform sampler2D TextureSpec;
uniform sampler2D TextureCloud;
uniform int UseTexture;
uniform int IsEarth;
uniform int EnableCloud;

void main() 
{ 
	fColor = color;
} 
