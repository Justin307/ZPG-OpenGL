#include "Scene.h"

Scene::Scene()
{
}

void Scene::AddModel(DrawableObject* model)
{
	models.push_back(model);
}

void Scene::SetSkybox(DrawableObject* model)
{
	this->skybox = model;
}

void Scene::Render()
{
	if (this->skybox != nullptr)
	{
		this->skybox->Render();	
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	for (DrawableObject* model : models)
	{
		model->Render();
	}
}
