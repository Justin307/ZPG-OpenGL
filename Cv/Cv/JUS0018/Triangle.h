#pragma once
#include "Model.h"
class Triangle : public Model
{
private:

public:
	Triangle(float points[], uint8_t size);
	void RenderMe();
};