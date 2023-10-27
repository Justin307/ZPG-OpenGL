#include "Shader.h"
#include "Camera.h"
#include <string>
#include <fstream>
#include <sstream>

#include "glm/gtc/type_ptr.hpp"

Shader* Shader::LoadFromFile(std::string vertex, std::string fragment)
{
    return LoadFromFile(vertex.c_str(), fragment.c_str());
}

Shader* Shader::LoadFromFile(const char* vertex, const char* fragment)
{
    std::ifstream v(vertex);
    std::stringstream v_buffer;
    v_buffer << v.rdbuf();
    std::ifstream f(fragment);
    std::stringstream f_buffer;
    f_buffer << f.rdbuf();
    std::string v_str = v_buffer.str();
    std::string f_str = f_buffer.str();
    return new Shader(v_str.c_str(), f_str.c_str());
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
        glProgramUniformMatrix4fv(shaderProgram, uniformLocationId, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

void Shader::SetUniformLocationValue(char* location, glm::mat4 matrix)
{
    GLint uniformLocationId = GetUniformLocation(location);
    if (uniformLocationId >= 0)
    {
        glProgramUniformMatrix4fv(shaderProgram, uniformLocationId, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

void Shader::SetUniformLocationValue(std::string location, glm::vec3 vector)
{
    GLint uniformLocationId = GetUniformLocation(location);
    if (uniformLocationId >= 0)
    {
        glProgramUniform3f(shaderProgram, uniformLocationId, vector.x, vector.y, vector.z);
    }
}

void Shader::SetUniformLocationValue(char* location, glm::vec3 vector)
{
    GLint uniformLocationId = GetUniformLocation(location);
    if (uniformLocationId >= 0)
    {
        glProgramUniform3f(shaderProgram, uniformLocationId, vector.x, vector.y, vector.z);
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
    this->SetUniformLocationValue(std::string("projectionMatrix"), this->camera->GetProjection());
    this->SetUniformLocationValue(std::string("viewMatrix"), this->camera->GetView());
}
