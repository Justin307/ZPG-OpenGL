#include "Light.h"

Light::Light()
{
}

Light::Light(glm::vec4 color) : color(color)
{
}

Light::Light(glm::vec4 color, float constant, float linear, float quadratic) : 
	color(color), constant(constant), linear(linear), quadratic(quadratic)
{
}


void Light::SetColor(glm::vec4 color)
{
	this->color = color;
	NotifyObservers();
}

void Light::SetAtteuation(float constant, float linear, float quadratic)
{
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
	NotifyObservers();
}

PositionedLight::PositionedLight()
{
}

PositionedLight::PositionedLight(glm::vec4 color, glm::vec3 position) :
	Light(color), position(position)
{
}

PositionedLight::PositionedLight(glm::vec4 color, float constant, float linear, float quadratic, glm::vec3 position) :
	Light(color, constant, linear, quadratic), position(position)
{
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
	data.constant = this->constant;
	data.linear = this->linear;
	data.quadratic = this->quadratic;
	data.position = this->position;
	return data;
}
