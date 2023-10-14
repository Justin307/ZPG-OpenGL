#include "Shader.h"

Shader::Shader(const char* vertex_shader, const char* fragment_shader)
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

void Shader::Use()
{
    glUseProgram(shaderProgram);
}

GLint Shader::GetUniformLocation(std::string name)
{
    return glGetUniformLocation(this->shaderProgram, name.c_str());
}

GLint Shader::GetUniformLocation(char* name)
{
    return glGetUniformLocation(this->shaderProgram, name);
}

void Shader::SetUniformLocationValue(std::string location, glm::mat4 matrix)
{
    GLint uniformLocationId = GetUniformLocation(location);
    if (uniformLocationId >= 0)
    {
        glUniformMatrix4fv(uniformLocationId, 1, GL_FALSE, &matrix[0][0]);
    }
}

void Shader::SetUniformLocationValue(char* location, glm::mat4 matrix)
{
    GLint uniformLocationId = GetUniformLocation(location);
    if (uniformLocationId >= 0)
    {
        glUniformMatrix4fv(uniformLocationId, 1, GL_FALSE, &matrix[0][0]);
    }
}

void Shader::CheckShader()
{
    GLint status;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(shaderProgram, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
    }
}
