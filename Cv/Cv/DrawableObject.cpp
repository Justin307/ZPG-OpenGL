#include "DrawableObject.h"

DrawableObject::DrawableObject(Model* model, Shader* shader, Material* material, Texture* texture)
{
	this->model = model;
	this->shader = shader;
	this->material = material;
	this->transformation = nullptr;
	this->texture = texture;
}
DrawableObject::DrawableObject(Model* model, Shader* shader, Material* material, Texture* texture, TransformationComponent* transformation)
{
	this->model = model;
	this->shader = shader;
	this->material = material;
	this->texture = texture;
	this->transformation = transformation;
}

void DrawableObject::Render()
{
	shader->Use();
	texture->Use();
	glm::mat4 matrix = glm::mat4(1.0f);
	if (transformation != nullptr)
	{
		transformation->ApplyTransformation(matrix);
	}
	shader->SetUniformLocationValue(std::string("modelMatrix"), matrix);
	shader->SetUniformLocationValue(std::string("material.ambient"), material->ambient);
	shader->SetUniformLocationValue(std::string("material.diffuse"), material->diffuse);
	shader->SetUniformLocationValue(std::string("material.specular"), material->specular);
	shader->SetUniformLocationValue(std::string("material.shininess"), material->shininess);

	model->Render();
	shader->Unuse();
}

