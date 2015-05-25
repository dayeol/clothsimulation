#version 440

in   vec4 vPosition;

uniform mat4 ModelView;
uniform mat4 Projection;
uniform mat4 LookAt;

void main()
{
    gl_Position = Projection*LookAt*ModelView*vPosition;
}