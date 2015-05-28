#version 440
// per-fragment interpolated values from the vertex shader

in  vec4 color;
out vec4 fColor;

void main() 
{ 
	fColor = color;
} 
