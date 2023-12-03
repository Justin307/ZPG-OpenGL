#pragma once

#include "Model.h"
#include "Shader.h"
#include "TransformationComponent.h"
#include "Material.h"
#include "Texture.h"
#include "Movement.h"

class DrawableObject
{
private:
	static unsigned char next_id;
	Model* model;
	Shader* shader;
	Material* material;
	Texture* texture;
	TransformationComponent* transformation;
	Movement* movement;
	unsigned char id;

public:
	DrawableObject(Model* model, Shader* shader, Material* material, Texture* texture);
	DrawableObject(Model* model, Shader* shader, Material* material, Texture* texture, TransformationComponent* transformation);
	DrawableObject(Model* model, Shader* shader, Material* material, Texture* texture, Movement* movement);
	DrawableObject(Model* model, Shader* shader, Material* material, Texture* texture, TransformationComponent* transformation, Movement* movement);
	void Render();
	bool Identify(GLuint index);
};