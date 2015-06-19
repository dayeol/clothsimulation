#version 440

in vec4 vPosition;
out vec3 texCoords;

uniform mat4 Projection;
uniform mat4 LookAt;
uniform mat4 ModelView;


void main()
{
    gl_Position =   Projection * LookAt * ModelView * vPosition;
    texCoords = vPosition.xyz;
}  