#pragma once
#include <glm/ext/vector_float3.hpp>

class Material
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
};

