#include "App.h"

//Include GLEW
#include <GL/glew.h>
//Include GLFW
#include <GLFW/glfw3.h> 

//Include GLM
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Include the standard C++ headers  
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

//Include my headers
#include "Shader.h"
#include "TransformationComposite.h"
#include "TransformationRotate.h"
#include "TransformationScale.h"
#include "TransformationTranslate.h"
#include "Scene.h"
#include "Camera.h"

//Include models
#include "models/sphere.h"
#include "models/suzi_flat.h"
#include "models/suzi_smooth.h"

void App::error_callback(int error, const char* description) { fputs(description, stderr); }

void App::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        App::GetInstance()->camera->MoveFront();
    }
    else if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        App::GetInstance()->camera->MoveBack();
    }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        App::GetInstance()->camera->MoveLeft();
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        App::GetInstance()->camera->MoveRight();
    }
    else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
    {
        App::GetInstance()->camera->MoveUp();
    }
    else if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
    {
        App::GetInstance()->camera->MoveDown();
    }
}

void App::cursor_callback(GLFWwindow* window, double x, double y)
{
    double xDiff = App::GetInstance()->xPos - x;
    double yDiff = App::GetInstance()->yPos - y;
    App::GetInstance()->xPos = x;
    App::GetInstance()->yPos = y;
    App::GetInstance()->camera->MoveMouse(xDiff, yDiff);
}

const char* vertex_shader =
"#version 330\n"
"layout(location=0) in vec3 vp;"
"layout(location=1) in vec3 vertex_color;"
"uniform mat4 modelMatrix;"
"uniform mat4 viewMatrix;"
"uniform mat4 projectionMatrix;"
"out vec3 color;"
"void main () {"
"     gl_Position = projectionMatrix * viewMatrix * mat4(1.0) * vec4 (vp, 1.0);"
"     color = vertex_color;"
"}";

const char* fragment_shader =
"#version 330\n"
"in vec3 color;"
"out vec4 frag_colour;"
"void main () {"
"     frag_colour = vec4 (color, 1.0);"
"}";

const char* vertex_shader2 =
"#version 330\n"
"layout(location=0) in vec3 vp;"
"layout(location=1) in vec3 vertex_color;"
"uniform mat4 modelMatrix;"
"uniform mat4 viewMatrix;"
"uniform mat4 projectionMatrix;"
"out vec3 color;"
"void main () {"
"     gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4 (vp, 1.0);"
"     color = vertex_color;"
"}";

const char* fragment_shader2 =
"#version 330\n"
"in vec3 color;"
"out vec4 frag_colour;"
"void main () {"
"     frag_colour = vec4 (color, 1.0);"
"}";


void App::printGLEWInfo()
{
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    // get version info
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
    printf("Vendor %s\n", glGetString(GL_VENDOR));
    printf("Renderer %s\n", glGetString(GL_RENDERER));
    printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    printf("Using GLFW %i.%i.%i\n", major, minor, revision);
}

App::App()
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
    }

    //inicializace konkretni verze
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "ZPG", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);

    glfwSetKeyCallback(this->window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void App::run()
{
    //GLEW
    printGLEWInfo();

    TransformationComposite rotation;
    rotation.AddTransformation(new TransformationRotate(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    TransformationComposite translation;
    translation.AddTransformation(new TransformationTranslate(glm::vec3(0.0f, 0.0f, 0.5f)));
    TransformationComposite *transformation = new TransformationComposite();
    transformation->AddTransformation(&rotation);
    transformation->AddTransformation(&translation);
    //transformation->AddTransformation(new TransformationScale(glm::vec3(0.5f)));

    //Create shader program
    Shader* shaderWithMatrix = new Shader(vertex_shader, fragment_shader);
    Shader* shaderWithoutMatrix = new Shader(vertex_shader2, fragment_shader2);
    shaderWithMatrix->CheckShader();
    shaderWithoutMatrix->CheckShader();

    this->camera = new Camera();
    camera->AttachObserver(shaderWithoutMatrix);

    shaderWithoutMatrix->SetCamera(camera);

    //Create scene
    Scene* scene = new Scene();
    scene->AddModel(new DrawableObject(new Model(sphere, sizeof(sphere)), shaderWithoutMatrix));
    scene->AddModel(new DrawableObject(new Model(suziSmooth, sizeof(suziSmooth)), shaderWithoutMatrix, transformation));

    /*
    *   https://learnopengl.com/Advanced-OpenGL/Face-culling
    */

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    camera->MoveBack();
    camera->MoveBack();

    while (!glfwWindowShouldClose(window)) {
        // clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // render scene
        scene->Render();
        // update other events like input handling
        glfwPollEvents();
        // put the stuff we’ve been drawing onto the display
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}

App* App::app = 0;

App* App::GetInstance()
{
    if (!app)
        app = new App();
    return app;
}
