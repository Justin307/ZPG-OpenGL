#pragma once
#include <glm/ext/vector_float3.hpp>
class Movement
{
protected:
	float tMin;
	float tMax;
	float t;
	float speed;
	/*
	* True -> when T reaches TMax, T is reset to TMin
	* False -> when T reaches TMax/TMin, speed is multiplied by -1
	*/
	bool shouldReset;
public:
	Movement(float tMin, float tMax, float t, float speed, bool shouldReset);
	virtual glm::vec3 GetNext() = 0;
};

class LinearMovement : public Movement
{
private:
	glm::vec3 start;
	glm::vec3 end;
public:
	LinearMovement(float tMin, float tMax, float t, float speed, bool shouldReset, glm::vec3 start, glm::vec3 end);
	glm::vec3 GetNext();
};

class CircularMovement : public Movement
{
private:
	glm::vec3 center;
	float radius;
public:
	CircularMovement(float tMin, float tMax, float t, float speed, bool shouldReset, glm::vec3 center, float radius);
	glm::vec3 GetNext();
};

class BezierMovement : public Movement
{

};

