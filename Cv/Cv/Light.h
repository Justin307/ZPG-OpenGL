#pragma once
#include "Observable.h"
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

struct LightData
{
	int type;
	glm::vec4 color;
	float constant;
	float linear;
	float quadratic;
	glm::vec3 position;
	glm::vec3 direction;
};

class Light : public Observable
{
protected:
	glm::vec4 color{0.0f};
	float constant = 1.0;
	float linear = 0.09;
	float quadratic = 0.032;

public:
	Light();
	Light(glm::vec4 color);
	Light(glm::vec4 color, float constant, float linear, float quadratic);
	void NotifyObservers() = 0;
	void SetColor(glm::vec4 color);
	void SetAtteuation(float constant, float linear, float quadratic);
	virtual LightData GetData() = 0;
};

class PositionedLight : public Light
{
private:
	glm::vec3 position{0.0f};
public:
	PositionedLight();
	PositionedLight(glm::vec4 color, glm::vec3 position);
	PositionedLight(glm::vec4 color, float constant, float linear, float quadratic, glm::vec3 position);
	void NotifyObservers();
	void SetPosition(glm::vec3 position);
	LightData GetData();
};

