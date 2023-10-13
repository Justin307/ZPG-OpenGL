#pragma once
#include "TransformationComponent.h"

#include <vector>
#include <glm/ext/vector_float3.hpp>

class TransformationRotate : public TransformationComponent
{
private:
	float angle;
	glm::vec3 value;
public:
	TransformationRotate(float angle, glm::vec3 value);
	void ApplyTransformation(glm::mat4& matrix) override;
};