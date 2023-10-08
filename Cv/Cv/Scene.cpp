#include "Scene.h"

Scene::Scene()
{
}

void Scene::AddModel(Model* model)
{
	models.push_back(model);
}

void Scene::Render()
{
	for (Model* model : models)
	{
		model->Render();
	}
}
