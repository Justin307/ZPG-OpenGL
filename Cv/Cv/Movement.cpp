#include "Movement.h"
#include <glm/trigonometric.hpp>

Movement::Movement(float tMin, float tMax, float t, float speed, bool shouldReset) :
	tMin(tMin), tMax(tMax), t(t), speed(speed), shouldReset(shouldReset)
{
}

LinearMovement::LinearMovement(float tMin, float tMax, float t, float speed, bool shouldReset, glm::vec3 start, glm::vec3 end) :
	Movement(tMin, tMax, t, speed, shouldReset), start(start), end(end)
{
}

glm::vec3 LinearMovement::GetNext()
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
	return glm::vec3(start.x + (end.x - start.x) * t, start.y + (end.y - start.y) * t, start.z + (end.z - start.z) * t);
}

CircularMovement::CircularMovement(float tMin, float tMax, float t, float speed, bool shouldReset, glm::vec3 center, float radius) :
	Movement(tMin, tMax, t, speed, shouldReset), center(center), radius(radius)
{
}

glm::vec3 CircularMovement::GetNext()
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
	return glm::vec3(center.x + radius * cos(glm::radians(t)), center.y, center.z + radius * sin(glm::radians(t)));
}
