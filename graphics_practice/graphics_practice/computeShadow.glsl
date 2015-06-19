#version 430

layout(std430, binding = 1) buffer VertexPrevious{
	vec4 vertexPrevBuffer[];
};
layout(std430, binding = 2) buffer VertexCurrent{
	vec4 vertexCurrBuffer[];
};
layout(std430, binding = 3) buffer VertexOutput{
	vec4 vertexOutBuffer[];
};

void main()
{

}