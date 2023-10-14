#pragma once

#include "Model.h"

class DrawableObject
{
private:
	Model* model;
	Shader* shader;
	TransformationComponent* transformation;

public:
	DrawableObject(Model* model, Shader* shader);
	DrawableObject(Model* model, Shader* shader, TransformationComponent* transformation);
	void Render();

};