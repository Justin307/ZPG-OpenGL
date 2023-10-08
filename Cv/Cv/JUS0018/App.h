#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 

class App
{
private:
	GLFWwindow* window;
	int width;
	int height;
	void printGLEWInfo();
public: 
	App();
	void run();
};

