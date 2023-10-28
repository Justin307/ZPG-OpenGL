#pragma once

#include "Model.h"
#include "Shader.h"
#include "TransformationComponent.h"
#include "Material.h"

class DrawableObject
{
private:
	Model* model;
	Shader* shader;
	TransformationComponent* transformation;
	Material* material;

public:
	DrawableObject(Model* model, Shader* shader, Material* material);
	DrawableObject(Model* model, Shader* shader, Material* material, TransformationComponent* transformation);
	void Render();

};