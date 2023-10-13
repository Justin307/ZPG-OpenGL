#pragma once
#include "TransformationComponent.h"

#include <vector>
#include <glm/ext/vector_float3.hpp>

class TransformationTranslate : public TransformationComponent
{
private:
	glm::vec3 value;
public:
	TransformationTranslate(glm::vec3 value);
	void ApplyTransformation(glm::mat4& matrix) override;
};