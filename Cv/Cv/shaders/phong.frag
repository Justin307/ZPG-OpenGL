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
in vec3 cameraPos;

out vec4 frag_colour;

void main () 
{
	vec4 tex = texture(textureUnitId, uv);

	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);
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

			vec3 viewDir = normalize(cameraPos - worldPos3);

			float diff = max(dot(lightDir, normalize(worldNorm)),0.0);
			diffuse += (diff * vec4(material.diffuse,1.0f)) * light[i].color * attenuation  * tex;

			vec3 reflectDir = normalize( reflect ( - lightDir , normalize(worldNorm) ) );

			float spec;

			if(diff == 0)
				spec = 0;
			else
				spec = pow ( max ( dot ( viewDir , reflectDir ) , 0.0) , material.shininess);
	
			specular += (spec * vec4(material.specular,1.0f)) * light[i].color * attenuation;
		}
		//DirectionLight
		else if (light[i].type == 2)
		{
			vec3 lightDir = normalize(-light[i].direction);

			vec3 viewDir = normalize(cameraPos - worldPos3);

			float diff = max(dot(lightDir, normalize(worldNorm)),0.0);
			diffuse += (diff * vec4(material.diffuse,1.0f)) * light[i].color  * tex;

			vec3 reflectDir = normalize( reflect ( - lightDir , normalize(worldNorm) ) );

			float spec;

			if(diff == 0)
				spec = 0;
			else
				spec = pow ( max ( dot ( viewDir , reflectDir ) , 0.0) , material.shininess);
	
			specular += (spec * vec4(material.specular,1.0f)) * light[i].color;

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


				vec3 viewDir = normalize(cameraPos - worldPos3);

				float diff = max(dot(lightDir, normalize(worldNorm)),0.0);
				diffuse += (diff * vec4(material.diffuse,1.0f)) * light[i].color * attenuation * tex;

				vec3 reflectDir = normalize( reflect ( - lightDir , normalize(worldNorm) ) );

				float spec;

				if(diff == 0)
					spec = 0;
				else
					spec = pow ( max ( dot ( viewDir , reflectDir ) , 0.0) , material.shininess);
	
				specular += (spec * vec4(material.specular,1.0f)) * light[i].color * attenuation;
			}
		}
	}

	frag_colour = ambient + diffuse + specular;
}