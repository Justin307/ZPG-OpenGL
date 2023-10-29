#version 330

in vec4 worldPos;
in vec3 worldNor;
in vec3 color;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

out vec4 frag_colour;

void main () 
{
	frag_colour = vec4 ( normalize(material.ambient + material.diffuse + material.specular) , 1.0 );
}