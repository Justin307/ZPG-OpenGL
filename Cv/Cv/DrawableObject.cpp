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
	if (transformation != nullptr)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		transformation->ApplyTransformation(matrix);
		char id[] = "modelMatrix";
		shader->SetUniformLocationValue(id, matrix);
	}
	model->Render();
}

