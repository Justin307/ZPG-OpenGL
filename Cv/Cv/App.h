#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include "Camera.h"
#include "Light.h"
#include "Scene.h"
#include "TransformationTranslate.h"

class App
{
private:
	GLFWwindow* window;
	int width;
	int height;
	void printGLEWInfo();
	static void error_callback(int error, const char* description);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_callback(GLFWwindow* window, double x, double y);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void window_size_callback(GLFWwindow* window, int width, int height);


	App();
	static App* app;
	double yPos = 300;
	double xPos = 400;
	Scene* scene = nullptr;
	Camera* camera = nullptr;
	bool cameraMovement = true;
	ReflectorLight* flashLight = nullptr;
	TransformationTranslate* skyboxMovement = nullptr;
	//Left click helpers;
	Model* model = nullptr;
	Material* material = nullptr;
	Shader * shader = nullptr;
	Texture* texture = nullptr;

public: 
	void run();
	static App* GetInstance();
};

