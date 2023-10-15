#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include "Camera.h"

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
	App();
	static App* app;
	double yPos = 300;
	double xPos = 400;
	Camera* camera;

public: 
	void run();
	static App* GetInstance();
};

