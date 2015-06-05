#version 440
// per-fragment interpolated values from the vertex shader
//in  vec3 fN;
//in  vec3 fE;
//in  vec3 fL;
//in  vec2 texCoord;

//in  vec4 color;
out vec4 fColor;

//uniform sampler2D TextureColor;
//uniform sampler2D TextureNormal;
//uniform int isWireframe;

//uniform vec4 AmbientProduct;
//uniform vec4 DiffuseProduct;
//uniform vec4 SpecularProduct;
//uniform vec4 LightPosition;
//uniform float Shininess;

void main() 
{ 
	fColor = vec4(1,1,1,1);
} 
