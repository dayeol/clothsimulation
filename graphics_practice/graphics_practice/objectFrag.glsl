#version 440
// per-fragment interpolated values from the vertex shader
in  vec2 texCoord;
out vec4 fColor;

uniform sampler2D textureColor;
uniform int isFloor;

void main() 
{ 
	
	vec4 texColor = vec4(1,1,1,1);
	
	if(isFloor > 0)
	{
		texColor = texture2D(textureColor, texCoord.st);
		//texColor = vec4(0.5,0.5,1,1);
	}
	//texColor = texture2D(textureColor, texCoord.st);
	fColor = texColor;
} 
