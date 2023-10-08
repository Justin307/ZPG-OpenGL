#pragma once
#include <vector>
#include "Model.h"
class Scene
{
private:
	std::vector<Model*> models;
public:
	Scene();
	void AddModel(Model* model);
	void Render();
};

