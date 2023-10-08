#include "Transformation.h"
#include <glm/ext/matrix_transform.hpp>

Transformation* Transformation::Add(Transformation* transformation)
{
	this->transformation = transformation;
	return this->transformation;
}

TransformationScale::TransformationScale(glm::vec3 value) : value(value)
{
}

glm::mat4 TransformationScale::GetTransformationMatrix(glm::mat4 matrix)
{
	if (this->transformation != nullptr)
	{
		return transformation->GetTransformationMatrix(glm::scale(matrix, value));
	}
	else
		return glm::scale(matrix, value);
}

TransformationRotate::TransformationRotate(float angle, glm::vec3 value) : angle(angle), value(value)
{
}

glm::mat4 TransformationRotate::GetTransformationMatrix(glm::mat4 matrix)
{
	if (this->transformation != nullptr)
	{
		return transformation->GetTransformationMatrix(glm::rotate(matrix, angle, value));
	}
	else
		return glm::rotate(matrix, angle, value);
}

TransformationTranslate::TransformationTranslate(glm::vec3 value) :value(value)
{
}

glm::mat4 TransformationTranslate::GetTransformationMatrix(glm::mat4 matrix)
{
	if (this->transformation != nullptr)
	{
		return transformation->GetTransformationMatrix(glm::translate(matrix, value));
	}
	else
		return glm::translate(matrix, value);
}
