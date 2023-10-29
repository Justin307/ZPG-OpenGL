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
#include "models/plain.h"

void App::error_callback(int error, const char* description) { fputs(description, stderr); }

void App::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        App::GetInstance()->camera->MoveFront();
    }
    else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        App::GetInstance()->camera->MoveBack();
    }
    else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        App::GetInstance()->camera->MoveLeft();
    }
    else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        App::GetInstance()->camera->MoveRight();
    }
    else if (key == GLFW_KEY_LEFT_SHIFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        App::GetInstance()->camera->MoveUp();
    }
    else if (key == GLFW_KEY_LEFT_CONTROL && (action == GLFW_PRESS || action == GLFW_REPEAT))
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

    PositionedLight* light = new PositionedLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    light->AttachObserver(lambert);
    light->AttachObserver(phong);
    light->AttachObserver(blinn);
    light->NotifyObservers();

    Material* emerald = new Material(glm::vec3(0.0215, 0.1745, 0.0215),glm::vec3(0.07568, 0.61424, 0.07568), glm::vec3(0.633, 0.727811, 0.633), 32);
    Material* ruby = new Material(glm::vec3(0.1745, 0.01175, 0.01175),glm::vec3(0.61424, 0.04136, 0.04136), glm::vec3(0.727811, 0.626959, 0.626959), 32);
    Material* bronze = new Material(glm::vec3(0.2125, 0.1275, 0.054),glm::vec3(0.714, 0.4284, 0.18144), glm::vec3(0.393548, 0.271906, 0.166721), 8);
    Material* silver = new Material(glm::vec3(0.19225, 0.19225, 0.19225),glm::vec3(0.50754, 0.50754, 0.50754), glm::vec3(0.508273, 0.508273, 0.508273), 16);
    Material* brass = new Material(glm::vec3(0.329412, 0.223529, 0.027451), glm::vec3(0.780392, 0.568627, 0.113725), glm::vec3(0.992157, 0.941176, 0.807843), 8);
    Material* turquoise = new Material(glm::vec3(0.1, 0.18725, 0.1745),glm::vec3(0.396, 0.74151, 0.69102), glm::vec3(0.297254, 0.30829, 0.306678), 4);
    
    Scene* scene = new Scene();

#define SCENE 1

#if SCENE == 1
    //Phong's four balls
    TransformationTranslate* transformation1 = new TransformationTranslate(glm::vec3(2.0f, 0.0f, 0.0f));
    TransformationTranslate* transformation2 = new TransformationTranslate(glm::vec3(-2.0f, 0.0f, 0.0f));
    TransformationTranslate* transformation3 = new TransformationTranslate(glm::vec3(0.0f, 0.0f, 2.0f));
    TransformationTranslate* transformation4 = new TransformationTranslate(glm::vec3(0.0f, 0.0f, -2.0f));
    scene->AddModel(new DrawableObject(new Model(sphere, sizeof(sphere)), phong, emerald, transformation1));
    scene->AddModel(new DrawableObject(new Model(sphere, sizeof(sphere)), phong, emerald, transformation2));
    scene->AddModel(new DrawableObject(new Model(sphere, sizeof(sphere)), phong, emerald, transformation3));
    scene->AddModel(new DrawableObject(new Model(sphere, sizeof(sphere)), phong, emerald, transformation4));
#elif SCENE == 2
    //Solar system
    Model* sphereModel = new Model(sphere, sizeof(sphere));
    scene->AddModel(new DrawableObject(sphereModel, constant, ruby));
    //Mercury
    TransformationComposite* mercuryTransformation = new TransformationComposite();
    TransformationRotate* mercuryRotation = new TransformationRotate(glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
    mercuryTransformation->AddTransformation(mercuryRotation);
    mercuryTransformation->AddTransformation(new TransformationTranslate(glm::vec3(1.7, 0.0, 0.0)));
    mercuryTransformation->AddTransformation(new TransformationScale(glm::vec3(0.1, 0.1, 0.1)));
    scene->AddModel(new DrawableObject(sphereModel, blinn, silver, mercuryTransformation));
    //Venus
    TransformationComposite* venusTransformation = new TransformationComposite();
    TransformationRotate* venusRotation = new TransformationRotate(glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
    venusTransformation->AddTransformation(venusRotation);
    venusTransformation->AddTransformation(new TransformationTranslate(glm::vec3(2.33, 0.0, 0.0)));
    venusTransformation->AddTransformation(new TransformationScale(glm::vec3(0.2, 0.2, 0.2)));
    scene->AddModel(new DrawableObject(sphereModel, blinn, brass, venusTransformation));
    //Earth
    TransformationComposite* earthTransformation = new TransformationComposite();
    TransformationRotate* earthRotation = new TransformationRotate(glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
    earthTransformation->AddTransformation(earthRotation);
    earthTransformation->AddTransformation(new TransformationTranslate(glm::vec3(3, 0.0, 0.0)));
    earthTransformation->AddTransformation(new TransformationScale(glm::vec3(0.22, 0.22, 0.22)));
    scene->AddModel(new DrawableObject(sphereModel, blinn, emerald, earthTransformation));
    //Earth Moon
    TransformationComposite* moonTransformation = new TransformationComposite();
    moonTransformation->AddTransformation(earthTransformation);
    TransformationRotate* moonRotation = new TransformationRotate(glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
    moonTransformation->AddTransformation(moonRotation);
    moonTransformation->AddTransformation(new TransformationTranslate(glm::vec3(1.5, 0.0, 0.0)));
    moonTransformation->AddTransformation(new TransformationScale(glm::vec3(0.1, 0.1, 0.1)));
    scene->AddModel(new DrawableObject(sphereModel, blinn, ruby, moonTransformation));
    //Mars
    TransformationComposite* marsTransformation = new TransformationComposite();
    TransformationRotate* marsRotation = new TransformationRotate(glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
    marsTransformation->AddTransformation(marsRotation);
    marsTransformation->AddTransformation(new TransformationTranslate(glm::vec3(4, 0.0, 0.0)));
    marsTransformation->AddTransformation(new TransformationScale(glm::vec3(0.15, 0.15, 0.15)));
    scene->AddModel(new DrawableObject(sphereModel, blinn, bronze, marsTransformation));
    //Jupiter
    TransformationComposite* jupiterTransformation = new TransformationComposite();
    TransformationRotate* jupiterRotation = new TransformationRotate(glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
    jupiterTransformation->AddTransformation(jupiterRotation);
    jupiterTransformation->AddTransformation(new TransformationTranslate(glm::vec3(5.25, 0.0, 0.0)));
    jupiterTransformation->AddTransformation(new TransformationScale(glm::vec3(0.45, 0.45, 0.45)));
    scene->AddModel(new DrawableObject(sphereModel, blinn, bronze, jupiterTransformation));
    //Saturn
    TransformationComposite* saturnTransformation = new TransformationComposite();
    TransformationRotate* saturnRotation = new TransformationRotate(glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
    saturnTransformation->AddTransformation(saturnRotation);
    saturnTransformation->AddTransformation(new TransformationTranslate(glm::vec3(6.2, 0.0, 0.0)));
    saturnTransformation->AddTransformation(new TransformationScale(glm::vec3(0.30, 0.30, 0.30)));
    scene->AddModel(new DrawableObject(sphereModel, blinn, brass, saturnTransformation));
    //Uranus
    TransformationComposite* uranusTransformation = new TransformationComposite();
    TransformationRotate* uranusRotation = new TransformationRotate(glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
    uranusTransformation->AddTransformation(uranusRotation);
    uranusTransformation->AddTransformation(new TransformationTranslate(glm::vec3(7.1, 0.0, 0.0)));
    uranusTransformation->AddTransformation(new TransformationScale(glm::vec3(0.30, 0.30, 0.30)));
    scene->AddModel(new DrawableObject(sphereModel, blinn, turquoise, uranusTransformation));
    //Neptune
    TransformationComposite* neptuneTransformation = new TransformationComposite();
    TransformationRotate* neptuneRotation = new TransformationRotate(glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
    neptuneTransformation->AddTransformation(neptuneRotation);
    neptuneTransformation->AddTransformation(new TransformationTranslate(glm::vec3(8, 0.0, 0.0)));
    neptuneTransformation->AddTransformation(new TransformationScale(glm::vec3(0.30, 0.30, 0.30)));
    scene->AddModel(new DrawableObject(sphereModel, blinn, turquoise, neptuneTransformation));
    //Pluto
    TransformationComposite* plutoTransformation = new TransformationComposite();
    TransformationRotate* plutoRotation = new TransformationRotate(glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
    plutoTransformation->AddTransformation(plutoRotation);
    plutoTransformation->AddTransformation(new TransformationTranslate(glm::vec3(8.5, 0.0, 0.0)));
    plutoTransformation->AddTransformation(new TransformationScale(glm::vec3(0.05, 0.05, 0.05)));
    scene->AddModel(new DrawableObject(sphereModel, blinn, silver, plutoTransformation));
    //Comet
    TransformationComposite* cometTransformation = new TransformationComposite();
    cometTransformation->AddTransformation(new TransformationScale(glm::vec3(0.15, 0.15, 0.15)));
    cometTransformation->AddTransformation(new TransformationRotate(glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0)));
    TransformationTranslate* cometMovement = new TransformationTranslate(glm::vec3(10.0, -60.0, -10.0));
    cometTransformation->AddTransformation(cometMovement);
    scene->AddModel(new DrawableObject(new Model(gift, sizeof(gift)), lambert, ruby, cometTransformation));

#elif SCENE == 3 
    //Phong's light trimming
    Shader* phongwrong = Shader::LoadFromFile(std::string("..\\Cv\\shaders\\vertex.vert"), std::string("..\\Cv\\shaders\\phongwrong.frag"));
    camera->AttachObserver(phongwrong);
    light->AttachObserver(phongwrong);
    TransformationTranslate* transformation1 = new TransformationTranslate(glm::vec3(2.0f, 0.0f, 0.0f));
    TransformationTranslate* transformation2 = new TransformationTranslate(glm::vec3(-2.0f, 0.0f, 0.0f));
    emerald->shininess = 1;
    scene->AddModel(new DrawableObject(new Model(sphere, sizeof(sphere)), phong, emerald, transformation1));
    scene->AddModel(new DrawableObject(new Model(sphere, sizeof(sphere)), phongwrong, emerald, transformation2));
#elif SCENE == 4
    //Projection matrix testing
    TransformationTranslate* transformation1 = new TransformationTranslate(glm::vec3(6.0f, 0.0f, 0.0f));
    TransformationTranslate* transformation2 = new TransformationTranslate(glm::vec3(-4.0f, 0.0f, 0.0f));
    TransformationTranslate* transformation3 = new TransformationTranslate(glm::vec3(2.0f, -2.0f, 0.0f));
    TransformationTranslate* transformation4 = new TransformationTranslate(glm::vec3(-2.0f, 0.0f, 0.0f));
    scene->AddModel(new DrawableObject(new Model(sphere, sizeof(sphere)), lambert, emerald, transformation1));
    scene->AddModel(new DrawableObject(new Model(gift, sizeof(gift)), lambert, emerald, transformation2));
    scene->AddModel(new DrawableObject(new Model(tree, sizeof(tree)), lambert, emerald, transformation3));
    scene->AddModel(new DrawableObject(new Model(suziSmooth, sizeof(suziSmooth)), lambert, emerald, transformation4));
    light->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
#elif SCENE == 5
    //Forest
    light->SetPosition(glm::vec3(0.0f, 0.25f, 0.0f));
    TransformationScale* planeScale = new TransformationScale(glm::vec3(50.0f, 1.0f, 50.0f));
    TransformationTranslate* planeTranslate = new TransformationTranslate(glm::vec3(0.0f, -1.0f, 0.0f));
    TransformationComposite* planeTransformations = new TransformationComposite();
    planeTransformations->AddTransformation(planeScale);
    planeTransformations->AddTransformation(planeTranslate);
    scene->AddModel(new DrawableObject(new Model(plain, sizeof(plain)), lambert, emerald, planeTransformations));
    TransformationComposite* bushesTransformation = new TransformationComposite();
    bushesTransformation->AddTransformation(new TransformationTranslate(glm::vec3(0.0f, -1.0f, 0.0f)));
    bushesTransformation->AddTransformation(new TransformationRotate(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    scene->AddModel(new DrawableObject(new Model(bushes, sizeof(bushes)), lambert, ruby, bushesTransformation));
    bushesTransformation = new TransformationComposite();
    bushesTransformation->AddTransformation(new TransformationTranslate(glm::vec3(0.0f, -1.0f, 0.0f)));
    bushesTransformation->AddTransformation(new TransformationRotate(glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    scene->AddModel(new DrawableObject(new Model(bushes, sizeof(bushes)), lambert, bronze, bushesTransformation));
    bushesTransformation = new TransformationComposite();
    bushesTransformation->AddTransformation(new TransformationTranslate(glm::vec3(0.0f, -1.0f, 0.0f)));
    bushesTransformation->AddTransformation(new TransformationRotate(glm::radians(240.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    scene->AddModel(new DrawableObject(new Model(bushes, sizeof(bushes)), lambert, brass, bushesTransformation));
    TransformationComposite* sphereTransformation = new TransformationComposite();
    sphereTransformation->AddTransformation(new TransformationTranslate(glm::vec3(0.75f, -1.0f, 0.75f)));
    sphereTransformation->AddTransformation(new TransformationScale(glm::vec3(0.2f, 0.2f, 0.2f)));
    scene->AddModel(new DrawableObject(new Model(sphere, sizeof(sphere)), lambert, silver, sphereTransformation));
    sphereTransformation = new TransformationComposite();
    sphereTransformation->AddTransformation(new TransformationTranslate(glm::vec3(0.75f, -1.0f, -0.75f)));
    sphereTransformation->AddTransformation(new TransformationScale(glm::vec3(0.2f, 0.2f, 0.2f)));
    scene->AddModel(new DrawableObject(new Model(sphere, sizeof(sphere)), lambert, silver, sphereTransformation));
    TransformationComposite* suziTransformation = new TransformationComposite();
    suziTransformation->AddTransformation(new TransformationTranslate(glm::vec3(0.75f, -0.5f, 0.75f)));
    suziTransformation->AddTransformation(new TransformationScale(glm::vec3(0.2f, 0.2f, 0.2f)));
    suziTransformation->AddTransformation(new TransformationRotate(glm::radians(-135.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    scene->AddModel(new DrawableObject(new Model(suziSmooth, sizeof(suziSmooth)), blinn, bronze, suziTransformation));
    suziTransformation = new TransformationComposite();
    suziTransformation->AddTransformation(new TransformationTranslate(glm::vec3(0.75f, -0.5f, -0.75f)));
    suziTransformation->AddTransformation(new TransformationScale(glm::vec3(0.2f, 0.2f, 0.2f)));
    suziTransformation->AddTransformation(new TransformationRotate(glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    scene->AddModel(new DrawableObject(new Model(suziFlat, sizeof(suziFlat)), blinn, brass, suziTransformation));
    TransformationComposite* giftTranformation = new TransformationComposite();
    giftTranformation->AddTransformation(new TransformationTranslate(glm::vec3(1.5f, -1.0f, 0.0f)));
    giftTranformation->AddTransformation(new TransformationRotate(glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    scene->AddModel(new DrawableObject(new Model(gift, sizeof(gift)), phong, ruby, giftTranformation));
    giftTranformation = new TransformationComposite();
    giftTranformation->AddTransformation(new TransformationTranslate(glm::vec3(1.8f, -1.0f, 0.3f)));
    giftTranformation->AddTransformation(new TransformationRotate(glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    scene->AddModel(new DrawableObject(new Model(gift, sizeof(gift)), phong, bronze, giftTranformation));
    giftTranformation = new TransformationComposite();
    giftTranformation->AddTransformation(new TransformationTranslate(glm::vec3(1.8f, -1.0f, -0.3f)));
    giftTranformation->AddTransformation(new TransformationRotate(glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f))); 
    scene->AddModel(new DrawableObject(new Model(gift, sizeof(gift)), phong, silver, giftTranformation));
    giftTranformation = new TransformationComposite();
    giftTranformation->AddTransformation(new TransformationTranslate(glm::vec3(2.1f, -1.0f, 0.0f)));
    giftTranformation->AddTransformation(new TransformationRotate(glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    scene->AddModel(new DrawableObject(new Model(gift, sizeof(gift)), phong, brass, giftTranformation));
    srand(time(nullptr));
    Model* treeModel = new Model(tree, sizeof(tree));
    for (int i = 0; i < 120; i++)
    {
        float x = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (30)))) - 15;
        float z = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (30)))) - 15;
        if ((3.0f > x && x >= -3.0f) && (3.0f > z && z >= -3.0f))
            continue;
        TransformationComposite* treeTransformation = new TransformationComposite();
        treeTransformation->AddTransformation(new TransformationTranslate(glm::vec3(x, -1.0f, z)));
        float scale = 0.25 + static_cast <float> (rand() / (static_cast <float> (RAND_MAX)));
        treeTransformation->AddTransformation(new TransformationScale(glm::vec3(scale, scale, scale)));
        float rotation = static_cast <float> (rand() / (static_cast <float> (RAND_MAX/360)));
        treeTransformation->AddTransformation(new TransformationRotate(glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)));
        scene->AddModel(new DrawableObject(treeModel, lambert, emerald, treeTransformation));
    }
#endif

    while (!glfwWindowShouldClose(window)) {
        // clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // render scene
        scene->Render();
#if SCENE == 2
        //Planets
        mercuryRotation->angle += 0.0199;
        venusRotation->angle += 0.0145;
        earthRotation->angle += 0.0124;
        marsRotation->angle += 0.0100;
        jupiterRotation->angle += 0.0054;
        saturnRotation->angle += 0.0040;
        uranusRotation->angle += 0.0028;
        neptuneRotation->angle += 0.0023;
        plutoRotation->angle += 0.0020;
        //Moon
        moonRotation->angle += 0.05;
        //Comet
        cometMovement->value.y += 0.05;
#endif
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
