#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h> 

class Model
{
protected:
	GLuint VAO;
	GLuint VBO;
	uint32_t verticeCount;
public:
	Model(float points[], uint8_t size);
	void Render();
};

