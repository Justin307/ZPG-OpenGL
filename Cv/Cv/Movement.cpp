#include "Movement.h"
#include <glm/trigonometric.hpp>

Movement::Movement(float tMin, float tMax, float t, float speed, bool shouldReset) :
	tMin(tMin), tMax(tMax), t(t), speed(speed), shouldReset(shouldReset)
{
}

void Movement::UpdateT()
{
	t += speed;
	if (t >= tMax || t <= tMin)
	{
		if (shouldReset && t <= tMin)
		{
			t = tMax;
		}
		else if (shouldReset && t >= tMax)
		{
			t = tMin;
		}
		else
		{
			speed *= -1;
		}
	}
}

LinearMovement::LinearMovement(float tMin, float tMax, float t, float speed, bool shouldReset, glm::vec3 start, glm::vec3 end) :
	Movement(tMin, tMax, t, speed, shouldReset), start(start), end(end)
{
}

glm::vec3 LinearMovement::GetNext()
{
	this->UpdateT();
	return glm::vec3(start.x + (end.x - start.x) * t, start.y + (end.y - start.y) * t, start.z + (end.z - start.z) * t);
}

CircularMovement::CircularMovement(float tMin, float tMax, float t, float speed, bool shouldReset, glm::vec3 center, float radius) :
	Movement(tMin, tMax, t, speed, shouldReset), center(center), radius(radius)
{
}

glm::vec3 CircularMovement::GetNext()
{
	this->UpdateT();
	return glm::vec3(center.x + radius * cos(glm::radians(t)), center.y, center.z + radius * sin(glm::radians(t)));
}

BezierMovement::BezierMovement(float tMin, float tMax, float t, float speed, bool shouldReset, glm::mat4 A, glm::mat4x3 B) :
	Movement(tMin, tMax, t, speed, shouldReset), A(A), B(B)
{
}

glm::vec3 BezierMovement::GetNext()
{
	this->UpdateT();
	glm::vec4 parameters = glm::vec4(t * t * t, t * t, t, 1.0f);
	glm::vec3 p = parameters * A * glm::transpose(B);
	return p;
}
