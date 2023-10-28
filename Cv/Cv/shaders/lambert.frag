#version 330

struct Light
{
	int type;
	vec4 color;
	vec3 position;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Light light;
uniform Material material;

in vec4 worldPos;
in vec3 worldNorm;
in vec3 color;

out vec4 frag_colour;

void main () 
{
   vec3 lightDir = light.position - vec3(worldPos);
   float diff = max(dot(normalize(lightDir), normalize(worldNorm)),0.0);
   vec4 diffuse = (diff * vec4(material.diffuse,1.0f)) * light.color;
   vec4 ambient = light.color * vec4(material.ambient, 1.0f);
   frag_colour = ambient + diffuse;
}