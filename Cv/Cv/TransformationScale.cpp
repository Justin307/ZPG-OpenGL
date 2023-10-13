#include "TransformationScale.h"
#include <glm/ext/matrix_transform.hpp>

TransformationScale::TransformationScale(glm::vec3 value) :value(value)
{
}

void TransformationScale::ApplyTransformation(glm::mat4& matrix)
{
	matrix = glm::scale(matrix, value);
}