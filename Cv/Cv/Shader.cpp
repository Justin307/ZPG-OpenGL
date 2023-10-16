#include "Shader.h"
#include "Camera.h"
#include <string>
#include <fstream>

std::string Shader::LoadFromFile(std::string filename)
{
    return LoadFromFile(filename.c_str());
}

std::string Shader::LoadFromFile(char* filename)
{
    //TODO
    std::string content;
    return content;
}

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

void Shader::SetCamera(Camera* camera)
{
    this->camera = camera;
    this->Update();
}

void Shader::Use()
{
    glUseProgram(shaderProgram);
    this->SetUniformLocationValue(std::string("projectionMatrix"), this->projectionMatrix);
    this->SetUniformLocationValue(std::string("viewMatrix"), this->viewMatrix);
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
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
        GLchar* strInfoLog = new GLchar[maxLength + 1];
        glGetShaderInfoLog(vertexShader, maxLength, NULL, strInfoLog);
        fprintf(stderr, "Compile failure: %s\n", strInfoLog);
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
        GLchar* strInfoLog = new GLchar[maxLength + 1];
        glGetShaderInfoLog(fragmentShader, maxLength, NULL, strInfoLog);
        fprintf(stderr, "Compile failure: %s\n", strInfoLog);
    }

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

void Shader::Update()
{
    this->viewMatrix = this->camera->GetView();
    this->projectionMatrix = this->camera->GetProjection();
}
