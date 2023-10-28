#include "Light.h"

Light::Light()
{
}

Light::Light(glm::vec4 color) : color(color)
{
}

void Light::SetColor(glm::vec4 color)
{
	this->color = color;
	NotifyObservers();
}

PositionedLight::PositionedLight()
{
}

PositionedLight::PositionedLight(glm::vec4 color, glm::vec3 position)
{
	this->color = color;
	this->position = position;
}

void PositionedLight::NotifyObservers()
{
	for (auto o : observers)
	{
		o->Update(POS_LIGHT, this);
	}
}

void PositionedLight::SetPosition(glm::vec3 position)
{
	this->position = position;
	NotifyObservers();
}

LightData PositionedLight::GetData()
{
	LightData data;
	data.type = 1;
	data.color = this->color;
	data.position = this->position;
	return data;
}
