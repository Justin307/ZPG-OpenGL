#version 330

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

uniform sampler2D textureUnitId;

in vec4 worldPos;
in vec3 worldNorm;
in vec3 color;
in vec2 uv;

out vec4 frag_colour;

void main () 
{
	frag_colour = texture(textureUnitId, uv);
}