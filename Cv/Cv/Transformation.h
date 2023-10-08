#pragma once
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
class Transformation
{
protected:
	Transformation* transformation;
public:
	virtual Transformation* Add(Transformation* transformation);
	virtual glm::mat4 GetTransformationMatrix(glm::mat4 matrix = glm::mat4(1.0f)) = 0;
};

class TransformationScale : public Transformation
{
private: 
	glm::vec3 value;
public:
	TransformationScale(glm::vec3 value);
	glm::mat4 GetTransformationMatrix(glm::mat4 matrix = glm::mat4(1.0f));
};

class TransformationRotate : public Transformation
{
private:
	float angle;
	glm::vec3 value;
public:
	TransformationRotate(float angle, glm::vec3 value);
	glm::mat4 GetTransformationMatrix(glm::mat4 matrix = glm::mat4(1.0f));
};

class TransformationTranslate : public Transformation
{
private:
	glm::vec3 value;
public:
	TransformationTranslate(glm::vec3 value);
	glm::mat4 GetTransformationMatrix(glm::mat4 matrix = glm::mat4(1.0f));
};

