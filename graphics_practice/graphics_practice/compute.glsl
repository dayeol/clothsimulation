#version 430

uniform float dt;
uniform int perRow;
uniform float structRest;
uniform float shearRest;
//kg
#define MASS 0.5
#define GRAVITY vec4(0.0,0.0,-0.02f, 0.0)
#define STIFFNESS 5
#define DAMPING	-0.5
#define DEFAULT_DAMPING -0.0255
#define WIND vec4(1.0, 0.0, 0.0, 0.0)

layout(std430, binding = 1) buffer VertexPrevious{
	vec4 vertexPrevBuffer[];
};
layout(std430, binding = 2) buffer VertexCurrent{
	vec4 vertexCurrBuffer[];
};
layout(std430, binding = 3) buffer VertexOutput{
	vec4 vertexOutBuffer[];
};
layout(std430, binding = 4) buffer Normal{
	vec3 normalBuffer[];
};


layout(local_size_x = 1024) in;

vec4 springForce(vec4 vel, vec4 velNeigh, vec4 a, vec4 b, float rest)
{
	vec4 deltaP = a - b;
	vec4 deltaV = vel - (velNeigh / dt);
	float d = distance(a, b);
	float stiff = (rest - d) *STIFFNESS;
	float damp = DAMPING* (dot(deltaV, deltaP) / d);
	return (stiff + damp) * normalize(deltaP);
}

float rand(vec2 n)
{
  return 0.5 + 0.5 * 
     fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

vec4 windForce(vec4 normal)
{
	vec4 n = normal;
	return dot(n, WIND) * WIND;
} 

void main() {
	int vertexIndex = int(gl_WorkGroupID*perRow + gl_LocalInvocationIndex);
	vec4 current = vertexCurrBuffer[vertexIndex];
	vec4 previous = vertexPrevBuffer[vertexIndex];
	vec4 currentNormal = vec4(normalBuffer[vertexIndex], 1);
	
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
	force += GRAVITY * mass + vel * DEFAULT_DAMPING;

	//structural force
	int rightIndex;
	rightIndex = vertexIndex + 1;
	if (rightIndex % perRow > col) // same row
	{
		vec4 right = vertexCurrBuffer[rightIndex];
		vec4 velNeigh = right - vertexPrevBuffer[rightIndex];
		force += springForce(vel, velNeigh, current, right, structRest);
	}
	int leftIndex = vertexIndex - 1;
	if (leftIndex % perRow < col)
	{
		vec4 left = vertexCurrBuffer[leftIndex];
		vec4 velNeigh = left - vertexPrevBuffer[leftIndex];
		force += springForce(vel, velNeigh, current, left, structRest);
	}
	int belowIndex = vertexIndex + perRow;
	if (belowIndex < perRow*perRow)
	{
		vec4 below = vertexCurrBuffer[belowIndex];
		vec4 velNeigh = below - vertexPrevBuffer[belowIndex];
		force += springForce(vel, velNeigh, current, below, structRest);
	}
	int upperIndex = vertexIndex - perRow;
	if (upperIndex >= 0)
	{
		vec4 upper;
		upper = vertexCurrBuffer[upperIndex];
		vec4 velNeigh = upper - vertexPrevBuffer[upperIndex];
		force += springForce(vel, velNeigh, current, upper, structRest);
	}

	//shear force
	int i1, i2, i3, i4;
	i1 = vertexIndex + 1 - perRow;
	i2 = vertexIndex - 1 - perRow;
	i3 = vertexIndex - 1 + perRow;
	i4 = vertexIndex + 1 + perRow;
	if (rightIndex % perRow > col && i1 >= 0)
	{
		vec4 shear1 = vertexCurrBuffer[i1];
		vec4 velNeigh = shear1 - vertexPrevBuffer[i1];
		force += springForce(vel, velNeigh, current, shear1, shearRest);
	}
	if (leftIndex % perRow < col && i2 >= 0)
	{
		vec4 shear2 = vertexCurrBuffer[i2];
		vec4 velNeigh = shear2 - vertexPrevBuffer[i2];
		force += springForce(vel, velNeigh, current, shear2, shearRest);
	}
	if (leftIndex % perRow < col && i3 < perRow*perRow)
	{
		vec4 shear3 = vertexCurrBuffer[i3];
		vec4 velNeigh = shear3 - vertexPrevBuffer[i3];
		force += springForce(vel, velNeigh, current, shear3, shearRest);
	}
	if (rightIndex % perRow > col && i4 < perRow*perRow)
	{
		vec4 shear4 = vertexCurrBuffer[i4];
		vec4 velNeigh = shear4 - vertexPrevBuffer[i4];
		force += springForce(vel, velNeigh, current, shear4, shearRest);
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
		vec4 velNeigh = up2 - vertexPrevBuffer[up2i];
		force += springForce(vel, velNeigh, current, up2, structRest * 2);
	}
	if (down2i < perRow*perRow)
	{
		vec4 down2 = vertexCurrBuffer[down2i];
		vec4 velNeigh = down2 - vertexPrevBuffer[down2i];
		force += springForce(vel, velNeigh, current, down2, structRest * 2);
	}
	if (right2i % perRow > col)
	{
		vec4 right2 = vertexCurrBuffer[right2i];
		vec4 velNeigh = right2 - vertexPrevBuffer[right2i];
		force += springForce(vel, velNeigh, current, right2, structRest * 2);
	}
	if (left2i %perRow < col)
	{
		vec4 left2 = vertexCurrBuffer[left2i];
		vec4 velNeigh = left2 - vertexPrevBuffer[left2i];
		force += springForce(vel, velNeigh, current, left2, structRest * 2);
	}


	// Calculate Normal
	vec3 normal = vec3(0.0, 0.0, 0.0);
	vec3 tangent;
	vec3 bitangent;

	if(col < colmax - 1)
	{
		tangent = normalize((vertexCurrBuffer[vertexIndex + 1] - vertexCurrBuffer[vertexIndex]).xyz);

		if(row < rowmax - 1)
		{
			bitangent = normalize((vertexCurrBuffer[vertexIndex + rowmax] - vertexCurrBuffer[vertexIndex]).xyz);
			normal += normalize(cross(bitangent, tangent));	
		}
		if(row > 0)
		{
			bitangent = normalize((vertexCurrBuffer[vertexIndex] - vertexCurrBuffer[vertexIndex - rowmax]).xyz);
			normal += normalize(cross(bitangent, tangent));	
		}
	}
	if(col > 0)
	{
		tangent = normalize((vertexCurrBuffer[vertexIndex] - vertexCurrBuffer[vertexIndex - 1]).xyz);

		if(row < rowmax - 1)
		{
			bitangent = normalize((vertexCurrBuffer[vertexIndex + rowmax] - vertexCurrBuffer[vertexIndex]).xyz);
			normal += normalize(cross(bitangent, tangent));	
		}
		if(row > 0)
		{
			bitangent = normalize((vertexCurrBuffer[vertexIndex] - vertexCurrBuffer[vertexIndex - rowmax]).xyz);
			normal += normalize(cross(bitangent, tangent));	
		}
	}

	normalBuffer[vertexIndex] = normalize(normal);

	vec2 rnd = vec2(0.0000008, 0.0000012);
	force += 0.02 * windForce(normalize(vec4(normal, 1))) + vel * DEFAULT_DAMPING;

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