#include "Scene.h"

Scene::Scene()
{
}

void Scene::AddModel(DrawableObject* model)
{
	models.push_back(model);
}

void Scene::Render()
{
	for (DrawableObject* model : models)
	{
		model->Render();
	}
}
