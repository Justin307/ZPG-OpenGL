#pragma once

#include "Model.h"
#include "Shader.h"
#include "TransformationComponent.h"
#include "Material.h"
#include "Texture.h"

class DrawableObject
{
private:
	Model* model;
	Shader* shader;
	TransformationComponent* transformation;
	Material* material;
	Texture* texture;

public:
	DrawableObject(Model* model, Shader* shader, Material* material, Texture* texture);
	DrawableObject(Model* model, Shader* shader, Material* material, Texture* texture, TransformationComponent* transformation);
	void Render();

};