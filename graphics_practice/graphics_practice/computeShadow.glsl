#version 430

uniform int row;
uniform int col;

layout(std430, binding = 1) buffer Vertices{
	vec4 vertexBuffer[];
}

layout(std430, binding = 2) buffer Shadow{
	vec4 shadowBuffer[];
}

vec4 calcShadow(vec4 pos)
{
	vec4 light = vec4(0, 0, 5, 1);
	vec4 diff = light - pos;

	float x = light.x; 
	float y = light.y;
	if(diff.z != 0)
	{
		x = diff.x * (-1 - light.z) / diff.z + light.x;
		y = diff.y * (-1 - light.z) / diff.z + light.y;
	}

	return vec4(x, y, -0.98, 1);
}

void main()
{
	int particles = (row + 1) * (col + 1);
	int vertexIndex = int(gl_WorkGroupID + gl_LocalInvocationIndex);
	vec4 currentShadow = shadowBuffer[vertexIndex];
	vec4 currentPos = vertexBuffer[vertexIndex];
	
	shadowBuffer[vertexIndex] = calcShadow(currentPos);
	barrier();
}