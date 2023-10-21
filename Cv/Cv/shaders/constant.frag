#version 330

in vec4 worldPos;
in vec3 worldNor;
in vec3 color;

out vec4 frag_colour;

void main () 
{
	frag_colour = vec4 ( 0.385 , 0.647 , 0.812 , 1.0 );
}