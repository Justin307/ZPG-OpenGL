#version 330

in vec4 worldPos;
in vec3 worldNor;

out vec4 frag_colour;

void main () 
{
   vec3 lightPosition = vec3(1.0, 1.0, 1.0);
   vec3 lightDir = lightPosition - worldPos.xyz;
   float diff = max(dot(normalize(lightDir), normalize(worldNor)),0.0);
   vec4 diffuse = diff * vec4(1.0,1.0,1.0,1.0);
   vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
   vec4 objectColor = vec4(0.385, 0.647, 0.812, 1.0);
   frag_colour = (ambient + diffuse) * objectColor;
}