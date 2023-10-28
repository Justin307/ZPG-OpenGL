#pragma once
#include "Observable.h"
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

struct LightData
{
	int type;
	glm::vec4 color;
	glm::vec3 position;
};

class Light : public Observable
{
protected:
	glm::vec4 color{0.0f};

public:
	Light();
	Light(glm::vec4 color);
	void NotifyObservers() = 0;
	void SetColor(glm::vec4 color);
	virtual LightData GetData() = 0;
};

class PositionedLight : public Light
{
private:
	glm::vec3 position{0.0f};
public:
	PositionedLight();
	PositionedLight(glm::vec4 color, glm::vec3 position);
	void NotifyObservers();
	void SetPosition(glm::vec3 position);
	LightData GetData();
};

