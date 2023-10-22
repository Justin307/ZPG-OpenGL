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
	Camera* camera = nullptr;;
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::mat4 projectionMatrix = glm::mat4(1.0f);;
	glm::vec3 cameraPosition = glm::vec3(0);
public:
	static Shader* LoadFromFile(std::string vertex, std::string fragment);
	static Shader* LoadFromFile(const char* vertex, const char* fragment);
	Shader(const char* vertex_shader, const char* fragment_shader);
	void SetCamera(Camera* camera);
	void Use();
	GLint GetUniformLocation(std::string name);
	GLint GetUniformLocation(char* name);
	void SetUniformLocationValue(std::string location, glm::mat4 matrix);
	void SetUniformLocationValue(char* location, glm::mat4 matrix);
	void SetUniformLocationValue(std::string location, glm::vec3 vector);
	void SetUniformLocationValue(char* location, glm::vec3 vector);
	void CheckShader();
	void Update();
};

