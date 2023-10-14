#pragma once
#include "Model.h"
class Rectangle : public Model
{
private:

public:
	Rectangle(float points[], uint8_t size);
	Rectangle(float points[], uint8_t size, Shader* shader);
	void Render();
};

