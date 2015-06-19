#version 440
in  vec4 color;
out vec4 fColor;

uniform sampler2D TextureColor;

void main()
{
	fColor = texture2D(TextureColor, vec2(0,0)) - vec4(0.8, 0.8, 0.8, 0);
}