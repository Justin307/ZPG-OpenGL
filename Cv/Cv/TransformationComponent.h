#pragma once
#include <vector>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

/*
* https://refactoring.guru/design-patterns/composite
*/

class TransformationComponent
{
public:
	virtual void ApplyTransformation(glm::mat4& matrix) = 0;
};