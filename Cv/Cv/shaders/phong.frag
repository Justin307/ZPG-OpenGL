#version 330

in vec4 worldPos;
in vec3 worldNor;
in vec3 color;
in vec3 cameraPos;

out vec4 frag_colour;

void main () 
{
	vec3 lightPosition = vec3(0.0, 0.0, 0.0);
	vec3 lightDir = lightPosition - vec3(worldPos);

	vec3 viewDir = cameraPos - vec3(worldPos);

	float specularStrength = 0.4;

	vec4 lightColor = vec4(1.0, 0.98, 0.54, 1);

	vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);

	//Using color instead of worldNor
	float diff = max(dot(normalize(lightDir), normalize(color)),0.0);
    vec4 diffuse = diff * vec4(0.385 ,0.647 ,0.812 ,1.0);

	//Using color instead of worldNor
	vec3 reflectDir = reflect ( - lightDir , color );

	float spec = pow ( max ( dot ( viewDir , reflectDir ) , 0.0) , 32);
	vec4 specular = specularStrength * spec * lightColor;

	vec4 objectColor = vec4 (0.385 ,0.647 ,0.812 ,1.0);

	frag_colour =( ambient + diffuse + specular )* objectColor ;
}