#pragma once
#include "Model.h"
class Rectangle : public Model
{
private:

public:
	Rectangle(float points[], uint8_t size);
	Rectangle(float points[], uint8_t size, Shader* shader);
	Rectangle(float points[], uint8_t size, Shader* shader, TransformationComponent* transformation);
	void Render(TransformationComponent* transformation, std::string location);
	void Render();
};

