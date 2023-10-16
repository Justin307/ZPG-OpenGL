#include "DrawableObject.h"

DrawableObject::DrawableObject(Model* model, Shader* shader)
{
	this->model = model;
	this->shader = shader;
	this->transformation = nullptr;
}
DrawableObject::DrawableObject(Model* model, Shader* shader, TransformationComponent* transformation)
{
	this->model = model;
	this->shader = shader;
	this->transformation = transformation;
}

void DrawableObject::Render()
{
	shader->Use();
	char id[] = "modelMatrix";
	glm::mat4 matrix = glm::mat4(1.0f);
	if (transformation != nullptr)
	{
		transformation->ApplyTransformation(matrix);
	}
	shader->SetUniformLocationValue(id, matrix);
	model->Render();
}

