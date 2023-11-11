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
#include <cstdlib>
#include <ctime>
#include <iostream>

//Include my headers
#include "Shader.h"
#include "TransformationComposite.h"
#include "TransformationRotate.h"
#include "TransformationScale.h"
#include "TransformationTranslate.h"
#include "Scene.h"
#include "Camera.h"
#include "Light.h"


//Include models
#include "models/sphere.h"
#include "models/suzi_flat.h"
#include "models/suzi_smooth.h"
#include "models/gift.h"
#include "models/tree.h"
#include "models/bushes.h"
//#include "models/plain.h"

void App::error_callback(int error, const char* description) { fputs(description, stderr); }

void App::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    App* app = App::GetInstance();
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        app->camera->MoveFront();
    }
    else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        app->camera->MoveBack();
    }
    else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        app->camera->MoveLeft();
    }
    else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        app->camera->MoveRight();
    }
    else if (key == GLFW_KEY_LEFT_SHIFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        app->camera->MoveUp();
    }
    else if (key == GLFW_KEY_LEFT_CONTROL && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        app->camera->MoveDown();
    }

    if (app->flashLight != nullptr)
    {
        app->flashLight->SetPosition(app->camera->GetPosition());
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
    if (app->flashLight != nullptr)
    {
        app->flashLight->SetDirection(app->camera->GetDirection());
        std::cout << app->flashLight->position.x << " " << app->flashLight->position.y << " " << app->flashLight->position.z << " ";
        std::cout << app->flashLight->direction.x << " " << app->flashLight->direction.y << " " << app->flashLight->direction.z << std::endl;
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

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void App::run()
{
    //GLEW
    printGLEWInfo();

    //Create shader program
    
    Shader* constant = Shader::LoadFromFile(std::string("..\\Cv\\shaders\\vertex.vert"), std::string("..\\Cv\\shaders\\constant.frag"));
    Shader* lambert = Shader::LoadFromFile(std::string("..\\Cv\\shaders\\vertex.vert"), std::string("..\\Cv\\shaders\\lambert.frag"));
    Shader* phong = Shader::LoadFromFile(std::string("..\\Cv\\shaders\\vertex.vert"), std::string("..\\Cv\\shaders\\phong.frag"));
    Shader* blinn = Shader::LoadFromFile(std::string("..\\Cv\\shaders\\vertex.vert"), std::string("..\\Cv\\shaders\\blinn.frag"));
    constant->CheckShader();
    lambert->CheckShader();
    phong->CheckShader();
    blinn->CheckShader();

    this->camera = new Camera();
    camera->AttachObserver(constant);
    camera->AttachObserver(lambert);
    camera->AttachObserver(phong);
    camera->AttachObserver(blinn);
    camera->NotifyObservers();

    /*PositionedLight* light = new PositionedLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 2.0f, 0.0f));
    light->AttachObserver(lambert);
    light->AttachObserver(phong);
    light->AttachObserver(blinn);
    light->NotifyObservers();*/
    ReflectorLight* light2 = new ReflectorLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.0, -1.0, 0.0), 20.0f);
    light2->AttachObserver(lambert);
    light2->AttachObserver(phong);
    light2->AttachObserver(blinn);
    light2->NotifyObservers();
    this->flashLight = light2;
    /*ReflectorLight* light4 = new ReflectorLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0, 0.0, 0.0), 2.0f);
    light4->AttachObserver(lambert);
    light4->AttachObserver(phong);
    light4->AttachObserver(blinn);
    light4->NotifyObservers();*/
    /*DirectionLight* light3 = new DirectionLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    light3->AttachObserver(lambert);
    light3->AttachObserver(phong);
    light3->AttachObserver(blinn);
    light3->NotifyObservers();*/

    Material* material = new Material(glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.8, 0.8, 0.8), glm::vec3(1.0, 1.0, 1.0), 32);
    
    Scene* scene = new Scene();

    const float plain[] = {
        //vrchol, normála, uv souøadnice
        1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
       -1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,

       -1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
       -1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f
    };

    scene->AddModel(new DrawableObject(new Model(plain, sizeof(plain)), lambert , material, new Texture("wall.jpg")));

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
