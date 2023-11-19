#pragma once
#include <vector>
#include "DrawableObject.h"
class Scene
{
private:
	DrawableObject* skybox = nullptr;
	std::vector<DrawableObject*> models;
public:
	Scene();
	void AddModel(DrawableObject* model);
	void SetSkybox(DrawableObject* model);
	void Render();
};

