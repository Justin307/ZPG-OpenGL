#pragma once

//Include GLEW
#include <GL/glew.h>
//Include GLFW
#include <GLFW/glfw3.h> 

//Include GLM
#include <glm/matrix.hpp>

#include <string>

class Shader
{
private:
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;
public:
	Shader(const char* vertex_shader, const char* fragment_shader);
	void Use();
	GLint GetUniformLocation(std::string name);
	GLint GetUniformLocation(char* name);
	void SetUniformLocationValue(std::string location, glm::mat4 matrix);
	void SetUniformLocationValue(char* location, glm::mat4 matrix);
};

