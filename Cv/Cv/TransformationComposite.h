#pragma once
#include "TransformationComponent.h"

#include <vector>
#include <glm/ext/vector_float3.hpp>

class TransformationComposite : public TransformationComponent
{
private:
	std::vector<TransformationComponent*> childs;
public:
	void ApplyTransformation(glm::mat4& matrix) override;
	void AddTransformation(TransformationComponent* transformation);
};