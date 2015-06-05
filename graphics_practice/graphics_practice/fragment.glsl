#version 440
// per-fragment interpolated values from the vertex shader
in  vec3 fN;
in  vec3 fE;
in  vec3 fL;
in  vec2 texCoord;

out vec4 fColor;

uniform sampler2D TextureColor;
uniform sampler2D TextureNormal;
uniform int isWireframe;

//uniform vec4 AmbientProduct;
//uniform vec4 DiffuseProduct;
//uniform vec4 SpecularProduct;
//uniform vec4 LightPosition;
//uniform float Shininess;

void main() 
{ 
	vec4 AmbientProduct = vec4(0.3, 0.3, 0.3, 1.0);
	vec4 DiffuseProduct = vec4(0.6, 0.6, 0.6, 1.0);
	vec4 SpecularProduct = vec4(0.35, 0.35, 0.35, 1.0);
	float Shininess = 10.0;
	vec4 texColor = texture2D(TextureColor, texCoord.st);

	vec3 N = normalize(fN);
	vec3 E = normalize(fE);
	vec3 L = normalize(fL);
	
	float shine = Shininess;
	
	// Specular effect
	vec3 H = normalize( L + E );

	//Normal mapping
	vec3 map = normalize(texture2D(TextureNormal, texCoord.st).rgb * 2.0 - 1.0);
	vec3 dp1 = dFdx(E);
	vec3 dp2 = dFdy(E);
	vec2 duv1 = dFdx(texCoord);
	vec2 duv2 = dFdy(texCoord);

	vec3 dp2perp = cross(dp2, N);
	vec3 dp1perp = cross(N, dp1);
	vec3 T = normalize(dp2perp * duv1.x + dp1perp * duv2.x);
	vec3 B = normalize(dp2perp * duv1.y + dp1perp * duv2.y);
	float invmax = inversesqrt(max(dot(T,T), dot(B,B)));
	mat3 TBN = mat3(T * invmax, B * invmax, N);
	N = normalize(TBN * map);

	vec4 ambient = AmbientProduct * texColor;

	float Kd = max(dot(L, N), 0.0);
	vec4 diffuse = Kd * texColor * DiffuseProduct;

	float Ks = pow(max(dot(N, H), 0.0), shine);
	vec4 specular = Ks * SpecularProduct;


	fColor =  ambient + diffuse;// + vec4(1,1,1,1);
	fColor.a = 1;
	
	if (isWireframe > 0)
		fColor = vec4(1,1,1,1);
	//fColor.a = 1.0;
	
} 
