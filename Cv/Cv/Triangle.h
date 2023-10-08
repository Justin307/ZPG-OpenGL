#pragma once
#include "Model.h"
class Triangle : public Model
{
private:

public:
	Triangle(float points[], uint8_t size);
	Triangle(float points[], uint8_t size, Shader* shader);
	Triangle(float points[], uint8_t size, Shader* shader, Transformation* transformation);
	void Render();
	void Render(Transformation* transformation, std::string location);
};