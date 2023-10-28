#pragma once

//Include GLEW
#include <GL/glew.h>
//Include GLFW
#include <GLFW/glfw3.h> 

//Include GLM
#include <glm/matrix.hpp>

#include <string>

#include "Observer.h"

class Camera;

class Shader : public Observer
{
private:
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;
public:
	static Shader* LoadFromFile(std::string vertex, std::string fragment);
	static Shader* LoadFromFile(const char* vertex, const char* fragment);
	Shader(const char* vertex_shader, const char* fragment_shader);
	void Use();
	void Unuse();
	GLint GetUniformLocation(std::string name);
	GLint GetUniformLocation(char* name);
	void SetUniformLocationValue(std::string location, glm::mat4 matrix);
	void SetUniformLocationValue(char* location, glm::mat4 matrix);
	void SetUniformLocationValue(std::string location, glm::vec3 vector);
	void SetUniformLocationValue(char* location, glm::vec3 vector);
	void SetUniformLocationValue(std::string location, glm::vec4 vector);
	void SetUniformLocationValue(char* location, glm::vec4 vector);
	void SetUniformLocationValue(std::string location, int value);
	void SetUniformLocationValue(char* location, int value);
	void SetUniformLocationValue(std::string location, float value);
	void SetUniformLocationValue(char* location, float value);
	void CheckShader();
	void Update(ObserverAction action, void* object);
};

