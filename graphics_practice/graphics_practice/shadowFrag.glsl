#version 440
in  vec4 color;
out vec4 fColor;

uniform int isWireframe;
uniform sampler2D TextureColor;

void main()
{
	if(isWireframe > 0)
		fColor = vec4(1,1,1,1);
	else
		fColor = texture2D(TextureColor, vec2(0,0)) - vec4(0.8, 0.8, 0.8, 0);
}