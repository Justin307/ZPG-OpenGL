#version 330

struct Light
{
	int type;
	vec4 color;
	float constant;
	float linear;
	float quadratic;
	vec3 position;
	vec3 direction;
	float angle;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Light light[10];
uniform int lightNumber;
uniform Material material;

uniform sampler2D textureUnitId;

in vec4 worldPos;
in vec3 worldNorm;
in vec3 color;
in vec2 uv;

out vec4 frag_colour;

void main () 
{
	vec4 tex = texture(textureUnitId, uv);

	vec4 diffuse = vec4(0.0);
	vec4 ambient = vec4(material.ambient, 1.0f) * tex;
	vec3 worldPos3 = worldPos.xyz / worldPos.w;
	for(int i = 0; i < lightNumber; i++)
	{
		//PositionedLight
		if(light[i].type == 1)
		{
			float distance = length(light[i].position - worldPos3);

			float attenuation = 1.0 / (light[i].constant + light[i].linear * distance + light[i].quadratic * distance * distance);

			vec3 lightDir = normalize(light[i].position - worldPos3);
			float diff = max(dot(lightDir, normalize(worldNorm)),0.0);
			diffuse += (diff * vec4(material.diffuse,1.0f)) * light[i].color * attenuation * tex;
		}
		//DirectionLight
		else if (light[i].type == 2)
		{
			vec3 lightDir = -normalize(light[i].direction);
			float diff = max(dot(lightDir, normalize(worldNorm)),0.0);
			diffuse += (diff * vec4(material.diffuse,1.0f)) * light[i].color * tex;
		}
		//ReflectorLight
		else if (light[i].type == 3)
		{
			vec3 lightDir = normalize(light[i].position - worldPos3);

			float angle = dot(-lightDir, normalize(light[i].direction));
			if(angle >= cos(radians(light[i].angle)))
			{
				float distance = length(light[i].position - worldPos3);

				float attenuation = 1.0 / (light[i].constant + light[i].linear * distance + light[i].quadratic * distance * distance);
				
				float diff = max(dot(lightDir, normalize(worldNorm)),0.0);
				diffuse += (diff * vec4(material.diffuse,1.0f)) * light[i].color * attenuation * tex;
			}
		}
	}
	frag_colour = ambient + diffuse;
}