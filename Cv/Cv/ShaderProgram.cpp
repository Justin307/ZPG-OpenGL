#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const char* vertex_shader, const char* fragment_shader)
{
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader, NULL);
    glCompileShader(vertexShader);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader, NULL);
    glCompileShader(fragmentShader);
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram, vertexShader);
    glLinkProgram(shaderProgram);
}

void ShaderProgram::UseMe()
{
    glUseProgram(shaderProgram);
}

GLint ShaderProgram::GetModelId()
{
    return glGetUniformLocation(this->shaderProgram, "modelMatrix");
}
