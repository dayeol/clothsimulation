#version 440

in   vec4 vPosition;
in   vec3 v_normal;
in	 vec2 vTexCoord;

out  vec3 fN;
out  vec3 fE;
out  vec3 fL;
out  vec2 texCoord;

uniform mat4 ModelView;
uniform mat4 Projection;
uniform mat4 LookAt;

void main()
{
    vec4 LightPosition = vec4(0, 0, 5.0, 1.0);
	mat3 normalMatrix = transpose(inverse(mat3(ModelView)));
	
	fN = (v_normal.xyz);
	fE = (inverse(LookAt) * vec4(0,0,0,1)).xyz - (ModelView*vPosition).xyz;
    fL = (LightPosition - ModelView*vPosition).xyz;

	texCoord = vTexCoord;
	gl_Position = Projection * LookAt * ModelView * vPosition;
}