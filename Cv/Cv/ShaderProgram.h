#pragma once

//Include GLEW
#include <GL/glew.h>
//Include GLFW
#include <GLFW/glfw3.h> 

class ShaderProgram
{
private:
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;
public:
	ShaderProgram(const char* vertex_shader, const char* fragment_shader);
	void UseMe();
	GLint GetModelId();
};

