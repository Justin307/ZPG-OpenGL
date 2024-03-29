#pragma once
#include "TransformationComponent.h"

#include <glm/ext/vector_float3.hpp>

class TransformationScale : public TransformationComponent
{
private:
public:
	glm::vec3 value;
	TransformationScale(glm::vec3 value);
	void ApplyTransformation(glm::mat4& matrix) override;
};