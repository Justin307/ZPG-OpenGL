#include "TransformationComposite.h"

void TransformationComposite::ApplyTransformation(glm::mat4& matrix)
{
	for (auto& transformation : this->childs)
	{
		transformation->ApplyTransformation(matrix);
	}
}

void TransformationComposite::AddTransformation(TransformationComponent* transformation)
{
	this->childs.push_back(transformation);
}