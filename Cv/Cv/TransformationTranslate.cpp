#include "TransformationTranslate.h"
#include <glm/ext/matrix_transform.hpp>

TransformationTranslate::TransformationTranslate(glm::vec3 value) :value(value)
{
}

void TransformationTranslate::ApplyTransformation(glm::mat4& matrix)
{
    matrix = glm::translate(matrix, this->value);
}