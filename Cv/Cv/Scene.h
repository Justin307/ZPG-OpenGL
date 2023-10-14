#pragma once
#include <vector>
#include "DrawableObject.h"
class Scene
{
private:
	std::vector<DrawableObject*> models;
public:
	Scene();
	void AddModel(DrawableObject* model);
	void Render();
};

