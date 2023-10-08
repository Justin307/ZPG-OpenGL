#pragma once
#include "Model.h"
class Rectangle : public Model
{
private:

public:
	Rectangle(float points[], uint8_t size);
	Rectangle(float points[], uint8_t size, Shader* shader);
	Rectangle(float points[], uint8_t size, Shader* shader, Transformation* transformation);
	void Render(Transformation* transformation, std::string location);
	void Render();
};

