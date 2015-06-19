#version 440

in vec3 texCoords;
out vec4 fColor;

uniform samplerCube Skybox;

void main()
{    
    fColor = texture(Skybox, texCoords);
	//fColor = vec4(1,1,1,1);
}
  