#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <string>

struct Vertex
{
	float Position[3];
	float Normal[3];
	float Texture[2];
	float Tangent[3];
};

class Model
{
protected:
	GLuint VAO;
	GLuint VBO;
	GLuint IBO;
	uint32_t indicesCount;
public:
	Model(std::string filename);
	void Render();
};

