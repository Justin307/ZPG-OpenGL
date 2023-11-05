#include "Light.h"

/*
*	Class Light
*/
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


/*
*	Class PositionedLight
*/
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

/*
*	Class DirectionLight
*/
DirectionLight::DirectionLight()
{
}

DirectionLight::DirectionLight(glm::vec4 color, glm::vec3 direction) :
	Light(color), direction(direction)
{
}

void DirectionLight::NotifyObservers()
{
	for (auto o : observers)
	{
		o->Update(DIR_LIGHT, this);
	}
}

void DirectionLight::SetDirection(glm::vec3 direction)
{
	this->direction = direction;
	NotifyObservers();
}

LightData DirectionLight::GetData()
{
	LightData data;
	data.type = 2;
	data.color = this->color;
	data.constant = this->constant;
	data.linear = this->linear;
	data.quadratic = this->quadratic;
	data.direction = this->direction;
	return data;
}

ReflectorLight::ReflectorLight()
{
}

ReflectorLight::ReflectorLight(glm::vec4 color, glm::vec3 position) :
	PositionedLight(color, position)
{
}

ReflectorLight::ReflectorLight(glm::vec4 color, glm::vec3 position, glm::vec3 direction, float angle) :
	PositionedLight(color, position), direction(direction), angle(angle)
{
}

ReflectorLight::ReflectorLight(glm::vec4 color, float constant, float linear, float quadratic, glm::vec3 position, glm::vec3 direction, float angle) :
	PositionedLight(color, constant, linear, quadratic, position), direction(direction), angle(angle)
{
}

void ReflectorLight::NotifyObservers()
{
	for (auto o : observers)
	{
		o->Update(REF_LIGHT, this);
	}
}

void ReflectorLight::SetDirection(glm::vec3 direction)
{
	this->direction = direction;
	NotifyObservers();
}

void ReflectorLight::SetAngle(float angle)
{
	this->angle = angle;
	NotifyObservers();
}

LightData ReflectorLight::GetData()
{
	LightData data;
	data.type = 3;
	data.color = this->color;
	data.constant = this->constant;
	data.linear = this->linear;
	data.quadratic = this->quadratic;
	data.position = this->position;
	data.direction = this->direction;
	data.angle = this->angle;
	return data;
}




