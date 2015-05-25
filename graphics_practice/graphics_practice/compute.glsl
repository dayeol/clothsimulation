#version 430

uniform float dt;
uniform int perRow;
//kg
#define MASS 1.0

#define GRAVITY vec4(0.0,0.0,-0.2, 0.0)
#define STIFFNESS 0.7

layout(std430, binding = 1) buffer VertexPrevious{
	vec4 vertexPrevBuffer[];
};
layout(std430, binding = 2) buffer VertexCurrent{
	vec4 vertexCurrBuffer[];
};
layout(std430, binding = 3) buffer VertexOutput{
	vec4 vertexOutBuffer[];
};

layout(local_size_x = 1024) in;

vec4 springForce(vec4 a, vec4 b, float distanceRest)
{
	vec4 delta = a - b;
	float d = length(delta);
	return 0.5 * delta * (distanceRest - d) / d * STIFFNESS;
}

void main() {
	int vertexIndex = int(gl_GlobalInvocationID.x);
	int column = vertexIndex % perRow;
	int row = vertexIndex / perRow;

	//calculate force
	vec4 force = vec4(0, 0, 0, 0);
	force += GRAVITY;
	vec4 acceleration = force / MASS;

	vec4 currentPosition = vertexCurrBuffer[vertexIndex];
	vec4 previousPosition = vertexPrevBuffer[vertexIndex];

	vertexOutBuffer[vertexIndex] = 2.0 * currentPosition - previousPosition + acceleration * dt * dt;

	barrier();
}