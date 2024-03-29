#version 330

struct Light
{
	int type;
	vec4 color;
	float constant;
	float linear;
	float quadratic;
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
in vec3 cameraPos;

out vec4 frag_colour;

void main()
{


	float distance = length(light.position - vec3(worldPos));

	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	vec3 lightDir = normalize(light.position - vec3(worldPos));

	vec3 viewDir = normalize(cameraPos - vec3(worldPos));

	vec4 ambient = light.color * vec4(material.ambient, 1.0f);

	float diff = dot(lightDir, normalize(worldNorm));
	vec4 diffuse = (diff * vec4(material.diffuse, 1.0f)) * light.color * attenuation;

	vec3 reflectDir = normalize(reflect(-lightDir, normalize(worldNorm)));

	float spec;

	if (diff == 0)
		spec = 0;
	else
		spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec4 specular = (spec * vec4(material.specular, 1.0f)) * light.color * attenuation;

	frag_colour = ambient + diffuse + specular;
}