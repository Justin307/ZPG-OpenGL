#include "TransformationRotate.h"
#include <glm/ext/matrix_transform.hpp>

TransformationRotate::TransformationRotate(float angle, glm::vec3 value) :angle(angle), value(value)
{
}

void TransformationRotate::ApplyTransformation(glm::mat4& matrix)
{
	matrix = glm::rotate(matrix, this->angle, this->value);
}