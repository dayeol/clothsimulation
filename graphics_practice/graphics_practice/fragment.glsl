#version 440
// per-fragment interpolated values from the vertex shader
in  vec3 fN;
in  vec3 fL;
in  vec3 fE;
//in  vec2 texCoord;
//in  vec2 cTexCoord;
//in	vec3 fTangent;
//in	vec3 fBitangent;

//in  vec4 color;
out vec4 fColor;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 ModelView;
uniform float Shininess;

void main() 
{ 
	
	vec3 N = normalize(fN);
	vec3 E = normalize(fE);
	vec3 L = normalize(fL);
	//vec4 texColor = texture2D(TextureColor, texCoord.st);
	float shine = Shininess;
	
	// Specular effect
	vec3 H = normalize( L + E );

	vec4 ambient = AmbientProduct;

	float Kd = max(dot(L, N), 0.0);
		
	vec4 diffuse = Kd * DiffuseProduct;
	float Ks = pow(max(dot(N, H), 0.0), shine);
	vec4 specular = Ks*SpecularProduct;


	fColor = vec4(0.5, 0.5, 0.5, 1) + ambient + diffuse + specular;
	fColor = vec4(1,1,1,1);
	fColor.a = 1.0;
	
} 
