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

out vec4 frag_colour;

void main () 
{
   vec3 lightDir = light.position - vec3(worldPos);
   float diff = max(dot(normalize(lightDir), normalize(worldNorm)),0.0);
   vec4 diffuse = diff * vec4(0.385 ,0.647 ,0.812 ,1.0);
   vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
   vec4 objectColor = vec4(0.385, 0.647, 0.812, 1.0);
   frag_colour = (ambient + diffuse) * objectColor;
}