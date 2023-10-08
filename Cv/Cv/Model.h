#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 

#include "Transformation.h"
#include "Shader.h"

class Model
{
protected:
	GLuint VAO = 0;
	GLuint VBO = 0;
	Shader* shader = 0;
	Transformation* transformation = 0;
public:
	virtual void Render() = 0;
	virtual void Render(Transformation* transformation, std::string location) = 0;
};

