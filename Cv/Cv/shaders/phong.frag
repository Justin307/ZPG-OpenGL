#version 330

struct Light
{
	int type;
	vec4 color;
	vec3 position;
};

uniform Light light;

in vec4 worldPos;
in vec3 worldNorm;
in vec3 color;
in vec3 cameraPos;

out vec4 frag_colour;

void main () 
{
	vec3 lightDir = normalize(light.position - vec3(worldPos));

	vec3 viewDir = normalize(cameraPos - vec3(worldPos));

	float specularStrength = 0.4;

	vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);

	float diff = max(dot(lightDir, normalize(worldNorm)),0.0);
    vec4 diffuse = diff * light.color;

	vec3 reflectDir = normalize( reflect ( - lightDir , normalize(worldNorm) ) );

	float spec;

	if(diff == 0)
		spec = 0;
	else
		spec = pow ( max ( dot ( viewDir , reflectDir ) , 0.0) , 64);
	
	vec4 specular = specularStrength * spec * light.color;

	vec4 objectColor = vec4(0.5,0.5,0.5, 1);

	frag_colour =( ambient + diffuse + specular )* objectColor ;
}