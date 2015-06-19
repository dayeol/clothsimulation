#version 430

uniform float dt;
uniform int perRow;
uniform int isWind;
uniform int isPin;
uniform float structRest;
uniform float shearRest;
uniform mat4 sphere;
uniform float sphereX;
uniform float kfr;

//kg
#define MASS 1
#define GRAVITY vec4(0.0,0.0,-0.04f, 0.0)


#define STRUCT_STIFF 30
#define STRUCT_DAMP	-0.5
#define SHEAR_STIFF 30
#define SHEAR_DAMP	-0.5
#define BEND_STIFF 15
#define BEND_DAMP	-0.5

#define GRAVITY_DAMPING -0.20
#define DEFAULT_DAMPING -0.25
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
	vec4 normalBuffer[];
};


layout(local_size_x = 1024) in;

vec4 springForce(vec4 vel, vec4 velNeigh, vec4 a, vec4 b, float rest, float stiffness, float damping)
{
	vec4 deltaP = a - b;
	vec4 deltaV = vel - (velNeigh / dt);
	float d = distance(a, b);
	float stiff = (rest - d) *stiffness;
	float damp = damping* (dot(deltaV, deltaP) / d);
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

struct neighbor
{
	int index;
	float rest;
	float stiff;
	float damp;
};

//neighbor types
#define NO_NEIGHBOR	-1
#define	UP 	0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define S1 4
#define S2 5
#define S3 6
#define S4 7
#define UP2 8
#define DOWN2 9
#define LEFT2 10
#define RIGHT2 11

bool isNeighbor(int index)
{
	return index != NO_NEIGHBOR;
}

vec3 gramSchmidt(vec3 u, vec3 v)
{
	vec3 uProjV;

	float vDotU;

	float uDotU = dot(u, u);

	if (uDotU == 0.0)
	{
		return vec3(0.0, 0.0, 0.0);
	}

	vDotU = dot(v, u);

	uProjV = u * (vDotU / uDotU);

	return v - uProjV;
}

void main() {
	neighbor neighbors[12];
	mat4 inv_sphere = inverse(sphere);
	int vertexIndex = int(gl_WorkGroupID*perRow + gl_LocalInvocationIndex);
	vec4 current = vertexCurrBuffer[vertexIndex];
	vec4 previous = vertexPrevBuffer[vertexIndex];
	vec4 currentNormal = normalBuffer[vertexIndex];
	
	int col = vertexIndex % perRow;
	int colmax = perRow; //column 마지막 번호
	int row = vertexIndex / perRow;
	int rowmax = perRow; //row 마지막 번호 = column 마지막 번호라고 한다. (row = column 라고 가정)
	float mass = MASS;
	vec4 vel = (current - previous) / dt;

	//calculate force
	vec4 force = vec4(0, 0, 0, 0);
	
	//pin
	if (isPin > 0)
	{
		//if ( vertexIndex == 0 || vertexIndex == perRow-1 || vertexIndex == perRow * perRow -1 || vertexIndex == perRow*(perRow-1))
		if ( vertexIndex == 0  || vertexIndex == perRow-1)
		{
			mass = 0;
		}
	}
	


	bool movable = true;

	//structural force
	neighbors[RIGHT].index = ((vertexIndex + 1)%perRow > col )?vertexIndex + 1:NO_NEIGHBOR;
	neighbors[LEFT].index = ((vertexIndex - 1) % perRow < col) ? vertexIndex - 1 : NO_NEIGHBOR;
	neighbors[DOWN].index = ((vertexIndex + perRow) < perRow*perRow) ? vertexIndex + perRow : NO_NEIGHBOR;
	neighbors[UP].index = ((vertexIndex - perRow) >= 0) ? vertexIndex - perRow : NO_NEIGHBOR;
	int rightIndex = neighbors[RIGHT].index;
	int leftIndex = neighbors[LEFT].index;
	int upIndex = neighbors[UP].index;
	int downIndex = neighbors[DOWN].index;
	//shear force
	neighbors[S1].index = (rightIndex != NO_NEIGHBOR && rightIndex - perRow >= 0) ? rightIndex - perRow:NO_NEIGHBOR;
	neighbors[S2].index = (leftIndex != NO_NEIGHBOR && leftIndex - perRow >= 0) ? leftIndex - perRow : NO_NEIGHBOR;
	neighbors[S3].index = (leftIndex != NO_NEIGHBOR && leftIndex + perRow < perRow*perRow) ? leftIndex + perRow : NO_NEIGHBOR;
	neighbors[S4].index = (rightIndex != NO_NEIGHBOR && rightIndex + perRow < perRow*perRow) ? rightIndex + perRow : NO_NEIGHBOR;
	//bending force
	neighbors[RIGHT2].index = ((vertexIndex + 2) % perRow > col) ? vertexIndex + 2 : NO_NEIGHBOR;
	neighbors[LEFT2].index = ((vertexIndex - 2) % perRow < col) ? vertexIndex - 2 : NO_NEIGHBOR;
	neighbors[DOWN2].index = ((vertexIndex + perRow * 2) < perRow*perRow) ? vertexIndex + perRow * 2 : NO_NEIGHBOR;
	neighbors[UP2].index = ((vertexIndex - perRow * 2) >= 0) ? vertexIndex - perRow * 2 : NO_NEIGHBOR;
	for (int i = 0; i < 4; i++)
	{
		neighbors[i].rest = structRest;
		neighbors[i].stiff = STRUCT_STIFF;
		neighbors[i].damp = STRUCT_DAMP;
	}
	for (int i = 4; i < 8; i++)
	{
		neighbors[i].rest = shearRest;
		neighbors[i].stiff = SHEAR_STIFF;
		neighbors[i].damp = SHEAR_DAMP;

	}
	for (int i = 8; i < 12; i++)
	{
		neighbors[i].rest = structRest * 2;
		neighbors[i].stiff = BEND_STIFF;
		neighbors[i].damp = BEND_DAMP;
	}
	//adding spring forces
	for (int i = 0; i <12; i++)
	{
		if (neighbors[i].index == NO_NEIGHBOR)
			continue;
		vec4 p_neighbor = vertexCurrBuffer[neighbors[i].index];
		vec4 v_neighbor = p_neighbor - vertexPrevBuffer[neighbors[i].index];
		force += springForce(vel, v_neighbor, current, p_neighbor, neighbors[i].rest,neighbors[i].stiff, neighbors[i].damp);
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

	normalBuffer[vertexIndex] = normalize(vec4(normal,1));

	float rnd = rand(vec2(0.1, 12)) *0.25 ;

	if (isWind > 0)
	{
		force += rnd * windForce(normalize(vec4(normal, 1))) + vel * DEFAULT_DAMPING;
	}

	force += GRAVITY * mass + vel * GRAVITY_DAMPING;

	//friction check
	vec4 sphereCenter = vec4(sphereX, 0, 0, 1);
	float currDist = length(current.xyz - sphereCenter.xyz);
	float radius = 0.51;
	//if (false)

	if (currDist <= radius+0.0005)
	{
		vec3 normal = normalize(current.xyz - sphereCenter.xyz);
		vec3 tangent = normalize(cross(cross(normal, force.xyz), normal));
		//수직항력
		float vertical = max(dot(force.xyz, -normal), 0);
		//수평성분 (force)
		float horizontal = max(dot(force.xyz, tangent), 0);
		//마찰계수

		if (vertical != 0)
		{
			//float kfr = 0.5;
			//마찰력0
			float friction = vertical * kfr;

			force = max(horizontal - friction, 0) * vec4(tangent, 0) + vel*-0.75;

			if (length(force) > 0 || vertical == 0)
				movable = true;
			else
			{
				movable = false;
			}
		}
		else
		{
			force += GRAVITY * mass + vel * GRAVITY_DAMPING;
		}
		
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

	if (movable)
		vertexOutBuffer[vertexIndex] = next;


	//vec4 next = 2.0 * current - previous + acceleration * dt * dt;

	barrier();

	//stretching constraint
	vec4 adjusted = next;
	if (leftIndex != NO_NEIGHBOR && mass != 0)
	{
		vec4 left = vertexOutBuffer[leftIndex];
		vec4 far = adjusted - left;
		if (length(far) > structRest * 1.1)
		{
			vec4 fix = normalize(-far)*(length(far) - structRest*1.1) / 2;
			adjusted = adjusted + fix;
		}
	}
	if (rightIndex != NO_NEIGHBOR && mass != 0)
	{
		vec4 left = vertexOutBuffer[rightIndex];
		vec4 far = adjusted - left;
		if (length(far) > structRest * 1.1)
		{
			vec4 fix = normalize(-far)*(length(far) - structRest*1.1) / 2;
			adjusted = adjusted + fix;
		}
	}
	if (upIndex != NO_NEIGHBOR && mass != 0)
	{
		vec4 up = vertexOutBuffer[upIndex];
		vec4 far = adjusted - up;
		if (length(far) > structRest * 1.1)
		{
			vec4 fix = normalize(-far)*(length(far) - structRest*1.1);
			adjusted = adjusted + fix;
		}
	}
	
	if (neighbors[S1].index != NO_NEIGHBOR && mass != 0)
	{
		vec4 left = vertexOutBuffer[neighbors[S1].index];
		vec4 far = adjusted - left;
		if (length(far) > shearRest * 1.2)
		{
			vec4 fix = normalize(-far)*(length(far) - shearRest*1.2);
			adjusted = adjusted + fix;
		}
	}
	if (neighbors[S2].index != NO_NEIGHBOR && mass != 0)
	{
		vec4 left = vertexOutBuffer[neighbors[S2].index];
		vec4 far = adjusted - left;
		if (length(far) > shearRest * 1.2)
		{
			vec4 fix = normalize(-far)*(length(far) - shearRest*1.2);
			adjusted = adjusted + fix;
		}
	}
	
	if (movable)
	{
		next = adjusted;
	}

	barrier();

	//collision with sphere
	vec3 d0 = next.xyz - vec3(sphereX, 0, 0); // center of sphere vec3(0, 0, 0)
	float dist = sqrt(d0.x * d0.x + d0.y * d0.y + d0.z * d0.z);
	vec4 X0 = inv_sphere * next;

	if (dist < radius)
	{
		d0 = (radius - dist) * d0 / dist;

		// Transform the delta back to originala space
		vec3 d = vec3(0.0, 0, 0);
		vec3 transformInv = vec3(sphere[0].x, sphere[1].x, sphere[2].x);
		transformInv /= dot(transformInv, transformInv);
		d.x = dot(d0, transformInv);

		transformInv = vec3(sphere[0].y, sphere[1].y, sphere[2].y);
		transformInv /= dot(transformInv, transformInv);
		d.y = dot(d0, transformInv);

		transformInv = vec3(sphere[0].z, sphere[1].z, sphere[2].z);
		transformInv /= dot(transformInv, transformInv);
		d.z = dot(d0, transformInv);

		next += vec4(d, 1);
		vertexOutBuffer[vertexIndex] = vec4(next.xyz, 1);
	}
	else
	{
		if (movable)
			vertexOutBuffer[vertexIndex] = next;
	}


	if(vertexOutBuffer[vertexIndex].z < -1)
	{
		vertexOutBuffer[vertexIndex] = vertexCurrBuffer[vertexIndex];
	}

	barrier();

}