#pragma once
#include "Model.h"
class Triangle : public Model
{
private:

public:
	Triangle(float points[], uint8_t size);
	Triangle(float points[], uint8_t size, Shader* shader);
	Triangle(float points[], uint8_t size, Shader* shader, TransformationComponent* transformation);
	void Render();
	void Render(TransformationComponent* transformation, std::string location);
};