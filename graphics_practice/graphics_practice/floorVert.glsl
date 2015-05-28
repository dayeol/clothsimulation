#version 440

in   vec4 vPosition;
in   vec4 vColor;

out  vec4 color;

uniform mat4 ModelView;
uniform mat4 Projection;
uniform mat4 LookAt;

void main()
{
	gl_Position = Projection*LookAt*ModelView*vPosition;
	color = vec4(1,1,1,1);
}