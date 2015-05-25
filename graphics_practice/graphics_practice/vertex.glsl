#version 440

in   vec4 vPosition;
in   vec3 vNormal;
in	 vec2 vTexCoord;
in   vec4 vColor;

// output values that will be interpretated per-fragment
out  vec3 fN;
out  vec3 fE;
out  vec3 fL;
out	 vec2 texCoord;
out  vec2 cTexCoord;
out  vec4 color;

uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform mat4 Projection;
uniform mat4 LookAt;
uniform vec2 EarthTime;

void main()
{

    gl_Position = Projection*LookAt*ModelView*vPosition;
	color = vColor;
}