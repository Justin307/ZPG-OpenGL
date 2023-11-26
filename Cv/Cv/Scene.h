#pragma once
#include <vector>
#include "DrawableObject.h"
class Scene
{
private:
	DrawableObject* skybox = nullptr;
	std::vector<DrawableObject*> models;
	Model* model = nullptr;
public:
	Scene();
	void AddModel(DrawableObject* model);
	void DeleteModel(GLuint index);
	void SetSkybox(DrawableObject* model);
	void Render();
};

