#include "DrawableObject.h"
#include <glm/ext/matrix_transform.hpp>

unsigned char DrawableObject::next_id = 0;

DrawableObject::DrawableObject(Model* model, Shader* shader, Material* material, Texture* texture)
{
	this->model = model;
	this->shader = shader;
	this->material = material;
	this->texture = texture;
	this->transformation = nullptr;
	this->movement = nullptr;
	id = next_id;
	next_id++;
}
DrawableObject::DrawableObject(Model* model, Shader* shader, Material* material, Texture* texture, TransformationComponent* transformation)
{
	this->model = model;
	this->shader = shader;
	this->material = material;
	this->texture = texture;
	this->transformation = transformation;
	this->movement = nullptr;
	id = next_id;
	next_id++;
}

DrawableObject::DrawableObject(Model* model, Shader* shader, Material* material, Texture* texture, Movement* movement)
{
	this->model = model;
	this->shader = shader;
	this->material = material;
	this->texture = texture;
	this->transformation = nullptr;
	this->movement = movement;
	id = next_id;
	next_id++;
}

DrawableObject::DrawableObject(Model* model, Shader* shader, Material* material, Texture* texture, TransformationComponent* transformation, Movement* movement)
{
	this->model = model;
	this->shader = shader;
	this->material = material;
	this->texture = texture;
	this->transformation = transformation;
	this->movement = movement;
	id = next_id;
	next_id++;
}

void DrawableObject::Render()
{
	shader->Use();
	texture->Use();
	glm::mat4 matrix = glm::mat4(1.0f);
	if (this->movement != nullptr)
	{
		auto temp = this->movement->GetNext();
		matrix = glm::translate(matrix, temp);
	}
	if (transformation != nullptr)
	{
		transformation->ApplyTransformation(matrix);
	}
	shader->SetUniformLocationValue(std::string("modelMatrix"), matrix);
	shader->SetUniformLocationValue(std::string("material.ambient"), material->ambient);
	shader->SetUniformLocationValue(std::string("material.diffuse"), material->diffuse);
	shader->SetUniformLocationValue(std::string("material.specular"), material->specular);
	shader->SetUniformLocationValue(std::string("material.shininess"), material->shininess);
	glStencilFunc(GL_ALWAYS, this->id, 0xFF);
	model->Render();
	shader->Unuse();
}

bool DrawableObject::Identify(GLuint index)
{
	return this->id == (unsigned char)index;
}
