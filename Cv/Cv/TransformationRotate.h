#pragma once
#include "TransformationComponent.h"

#include <glm/ext/vector_float3.hpp>

class TransformationRotate : public TransformationComponent
{
private:
public:
	float angle;
	glm::vec3 value;
	TransformationRotate(float angle, glm::vec3 value);
	void ApplyTransformation(glm::mat4& matrix) override;
};