#version 440

uniform int code;

out vec4 outputF;

void main()
{
	outputF = vec4(code/255.0);
}