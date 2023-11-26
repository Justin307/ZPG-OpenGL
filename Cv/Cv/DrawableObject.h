#pragma once

#include "Model.h"
#include "Shader.h"
#include "TransformationComponent.h"
#include "Material.h"
#include "Texture.h"

class DrawableObject
{
private:
	static char next_id;
	Model* model;
	Shader* shader;
	TransformationComponent* transformation;
	Material* material;
	Texture* texture;
	char id;

public:
	DrawableObject(Model* model, Shader* shader, Material* material, Texture* texture);
	DrawableObject(Model* model, Shader* shader, Material* material, Texture* texture, TransformationComponent* transformation);
	void Render();
	bool Identify(GLuint index);
};