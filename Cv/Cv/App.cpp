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
    App* app = App::GetInstance();
    if (app->cameraMovement)
    {
        double xDiff = app->xPos - x;
        double yDiff = app->yPos - y;
        app->xPos = x;
        app->yPos = y;
        app->camera->MoveMouse(xDiff, yDiff);
    }
}


void App::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        App* app = App::GetInstance();
        if (app->cameraMovement)
        {
            app->cameraMovement = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            //Move cursor to the center of the screen and center xPos/yPos
            double x = app->width / 2;
            double y = app->height / 2;
            glfwSetCursorPos(window, x, y);
            app->xPos = x;
            app->yPos = y;
        }
        else
        {
            app->cameraMovement = true;
            double x = app->width / 2;
            double y = app->height / 2;
            glfwSetCursorPos(window, x, y);
            app->xPos = x;
            app->yPos = y;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        
    }
        
}

void App::window_size_callback(GLFWwindow* window, int width, int height)
{
    App* app = App::GetInstance();
    app->width = width;
    app->height = height;
    glfwSetWindowSize(window, width, height);
    glViewport(0, 0, width, height);
    app->camera->SetWidthHeight(width, height);
}

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
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void App::run()
{
    //GLEW
    printGLEWInfo();

    TransformationTranslate* transformation1 = new TransformationTranslate(glm::vec3(2.0f, 0.0f, 0.0f));
    TransformationTranslate* transformation2 = new TransformationTranslate(glm::vec3(-2.0f, 0.0f, 0.0f));
    TransformationTranslate* transformation3 = new TransformationTranslate(glm::vec3(0.0f, 0.0f, 2.0f));
    TransformationTranslate* transformation4 = new TransformationTranslate(glm::vec3(0.0f, 0.0f, -2.0f));
    TransformationRotate* transformation5 = new TransformationRotate(glm::radians(45.0f),glm::vec3(0.0f, 1.0f, 0.0f));
    TransformationComposite* transformation6 = new TransformationComposite();
    transformation6->AddTransformation(transformation4);
    transformation6->AddTransformation(transformation5);

    //Create shader program
    
    Shader* constant = Shader::LoadFromFile(std::string("..\\Cv\\shaders\\vertex.vert"), std::string("..\\Cv\\shaders\\constant.frag"));
    Shader* lambert = Shader::LoadFromFile(std::string("..\\Cv\\shaders\\vertex.vert"), std::string("..\\Cv\\shaders\\lambert.frag"));
    Shader* phong = Shader::LoadFromFile(std::string("..\\Cv\\shaders\\vertex.vert"), std::string("..\\Cv\\shaders\\phong.frag"));
    constant->CheckShader();
    lambert->CheckShader();
    phong->CheckShader();

    this->camera = new Camera();
    camera->AttachObserver(constant);
    camera->AttachObserver(lambert);
    camera->AttachObserver(phong);

    constant->SetCamera(camera);
    lambert->SetCamera(camera);
    phong->SetCamera(camera);

    //Scene 1
    Scene* scene1 = new Scene();
    scene1->AddModel(new DrawableObject(new Model(sphere, sizeof(sphere)), lambert, transformation2));

    //Scene 2
    Scene* scene2 = new Scene();
    scene2->AddModel(new DrawableObject(new Model(sphere, sizeof(sphere)), constant, transformation1));
    scene2->AddModel(new DrawableObject(new Model(sphere, sizeof(sphere)), lambert, transformation2));
    scene2->AddModel(new DrawableObject(new Model(sphere, sizeof(sphere)), phong, transformation6));
    scene2->AddModel(new DrawableObject(new Model(sphere, sizeof(sphere)), constant, transformation3));
    
    //Scene 3
    Scene* scene3 = new Scene();
    scene3->AddModel(new DrawableObject(new Model(sphere, sizeof(sphere)), constant, transformation1));
    scene3->AddModel(new DrawableObject(new Model(suziFlat, sizeof(suziFlat)), lambert, transformation2));
    scene3->AddModel(new DrawableObject(new Model(suziSmooth, sizeof(suziSmooth)), phong, transformation6));

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
        scene3->Render();
        // update other events like input handling
        glfwPollEvents();
        // put the stuff we�ve been drawing onto the display
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
