#version 430

uniform float dt;
uniform int perRow;
uniform float structRest;
uniform float shearRest;
//kg
#define MASS 0.05

#define GRAVITY vec4(0.0,0.0,-0.02, 0.0)
#define STIFFNESS 2.0

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

vec4 springForce(vec4 prev, vec4 a, vec4 b, float rest)
{
	vec4 deltaP = a - b;
	float d = distance(a,b);
	return 0.5* deltaP*(rest - d) / d *STIFFNESS;
}

void main() {
	int vertexIndex = int(gl_GlobalInvocationID.x);
	vec4 current = vertexCurrBuffer[vertexIndex];
	vec4 previous = vertexPrevBuffer[vertexIndex];
	int col = vertexIndex % perRow;
	int colmax = perRow; //column 마지막 번호
	int row = vertexIndex / perRow;
	int rowmax = perRow; //row 마지막 번호 = column 마지막 번호라고 한다. (row = column 라고 가정)
	float mass = MASS;
	vec4 vel = (current - previous) / dt;

	//calculate force
	vec4 force = vec4(0, 0, 0, 0);
	
	//pin
	if ( vertexIndex == 0 || vertexIndex == perRow-1)
	{
		mass = 0;
	}

	//gravity
	force += GRAVITY * mass + vel*-0.0125f;

	//structural force
	int rightIndex;
	rightIndex = vertexIndex + 1;
	if (rightIndex % perRow > col) // same row
	{
		vec4 right = vertexCurrBuffer[rightIndex];
		force += springForce(previous, current, right, structRest);
	}
	int leftIndex = vertexIndex - 1;
	if (leftIndex % perRow < col)
	{
		vec4 left = vertexCurrBuffer[leftIndex];
		force += springForce(previous, current, left, structRest);
	}
	int belowIndex = vertexIndex + perRow;
	if (belowIndex < perRow*perRow)
	{
		vec4 below = vertexCurrBuffer[belowIndex];
		force += springForce(previous, current, below, structRest);
	}
	int upperIndex = vertexIndex - perRow;
	if (upperIndex >= 0)
	{
		vec4 upper = vertexCurrBuffer[upperIndex];
		force += springForce(previous, current, upper, structRest);
	}

	//shear force
	int i1, i2, i3, i4;
	i1 = vertexIndex + 1 - perRow;
	i2 = vertexIndex - 1 - perRow;
	i3 = vertexIndex - 1 + perRow;
	i4 = vertexIndex + 1 + perRow;
	if (rightIndex % perRow > col && i1 > 0)
	{
		vec4 shear1 = vertexCurrBuffer[i1];
		force += springForce(previous, current, shear1, shearRest);
	}
	if (leftIndex % perRow < col && i2 > 0)
	{
		vec4 shear2 = vertexCurrBuffer[i2];
		force += springForce(previous, current, shear2, shearRest);
	}
	if (leftIndex % perRow < col && i3 < perRow*perRow)
	{
		vec4 shear3 = vertexCurrBuffer[i3];
		force += springForce(previous, current, shear3, shearRest);
	}
	if (rightIndex % perRow > col && i4 < perRow*perRow)
	{
		vec4 shear4 = vertexCurrBuffer[i4];
		force += springForce(previous, current, shear4, shearRest);
	}

	//bending force
	int up2i, down2i, left2i, right2i;
	up2i = vertexIndex - perRow * 2;
	down2i = vertexIndex + perRow * 2;
	right2i = vertexIndex + 2;
	left2i = vertexIndex - 2; 
	if (up2i > 0)
	{
		vec4 up2 = vertexCurrBuffer[up2i];
		force += springForce(previous, current, up2, structRest * 2);
	}
	if (down2i < perRow*perRow)
	{
		vec4 down2 = vertexCurrBuffer[down2i];
		force += springForce(previous, current, down2, structRest * 2);
	}
	if (right2i % perRow > col)
	{
		vec4 right2 = vertexCurrBuffer[right2i];
		force += springForce(previous, current, right2, structRest * 2);
	}
	if (left2i %perRow < col)
	{
		vec4 left2 = vertexCurrBuffer[left2i];
		force += springForce(previous, current, left2, structRest * 2);
	}


	//verlet integration
	vec4 acceleration;
	if (mass == 0)
	{
		acceleration = vec4(0, 0, 0, 0);
	}
	else
	{
		acceleration = force / mass;
	}
	vec4 next = current + vel*dt + acceleration * dt * dt;
	//vec4 next = 2.0 * current - previous + acceleration * dt * dt;
	vertexOutBuffer[vertexIndex] = next;
	barrier();
}