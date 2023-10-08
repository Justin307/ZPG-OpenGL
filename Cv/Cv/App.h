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
	static void error_callback(int error, const char* description);

public: 
	App();
	void run();
};

