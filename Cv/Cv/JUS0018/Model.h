#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 

class Model
{
protected:
	GLuint VAO = 0;
	GLuint VBO = 0;
public:
	virtual void RenderMe() = 0;
};

