#version 440
// per-fragment interpolated values from the vertex shader
in  vec3 fN;
in  vec3 fE;
in  vec3 fL;
in  vec2 texCoord;
out vec4 fColor;

uniform sampler2D TextureColor;
uniform int isFloor;
uniform int isWireframe;

void main() 
{
	vec4 AmbientProduct = vec4(0.3, 0.3, 0.3, 1.0);
	vec4 DiffuseProduct = vec4(0.6, 0.6, 0.6, 1.0);
	vec4 SpecularProduct = vec4(0.35, 0.35, 0.35, 1.0);
	float Shininess = 10.0;
		
	vec3 N = normalize(fN);
	vec3 E = normalize(fE);
	vec3 L = normalize(fL);

	float shine = Shininess;

	vec4 texColor = vec4(1,1,1,1);
	
	texColor = texture2D(TextureColor, texCoord.st);
		
	// Specular effect
	vec3 H = normalize( L + E );

	vec4 ambient = AmbientProduct * texColor;

	float Kd = max(dot(L, N), 0.0);
	vec4 diffuse = Kd * texColor * DiffuseProduct;

	float Ks = pow(max(dot(N, H), 0.0), shine);
	vec4 specular = Ks * SpecularProduct;


	fColor =  ambient + diffuse + specular;
	fColor.a = 1;


	if (isWireframe > 0)
		fColor = vec4(1,1,1,1);
} 
