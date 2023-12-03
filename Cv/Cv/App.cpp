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
#include <cstdio>

//Include my headers
#include "Shader.h"
#include "TransformationComposite.h"
#include "TransformationRotate.h"
#include "TransformationScale.h"
#include "TransformationTranslate.h"
#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Movement.h"

void App::error_callback(int error, const char* description) { fputs(description, stderr); }

void App::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    App* app = App::GetInstance();
    if (app->camera != nullptr)
    {
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
        else if ((key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT || key == GLFW_KEY_SPACE) && (action == GLFW_PRESS || action == GLFW_REPEAT))
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

        if (app->skyboxMovement != nullptr)
        {
            app->skyboxMovement->value = app->camera->GetPosition();
        }
    }
}

void App::cursor_callback(GLFWwindow* window, double x, double y)
{
    App* app = App::GetInstance();
    if (app->camera != nullptr)
    {
        if (app->cameraMovement)
        {
            double xDiff = app->xPos - x;
            double yDiff = app->yPos - y;
            app->xPos = x;
            app->yPos = y;
            app->camera->MoveMouse(xDiff, yDiff);
        }
        else
        {
            app->xPos = x;
            app->yPos = y;
        }
        if (app->flashLight != nullptr)
        {
            app->flashLight->SetDirection(app->camera->GetDirection());
        }
    }
}


void App::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    App* app = App::GetInstance();
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
    {
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
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        GLuint index;

        GLint x = (GLint)app->xPos;
        GLint y = (GLint)app->yPos;

        int newy = app->camera->GetResolution().y - y;
        glReadPixels(x, newy, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

        printf("Trying to delete object with stencil index % u...\n", index);
        
        App::GetInstance()->scene->DeleteModel(index);
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        GLfloat depth;

        GLint x = (GLint)app->xPos;
        GLint y = (GLint)app->yPos;

        int newy = app->camera->GetResolution().y - y;

        glReadPixels(x, newy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

        glm::vec3 screenX = glm::vec3(x, newy, depth);
        glm::mat4 view = app->camera->GetView();
        glm::mat4 projection = app->camera->GetProjection();;
        glm::vec4 viewPort = glm::vec4(0, 0, app->camera->GetResolution().x, app->camera->GetResolution().y);
        glm::vec3 pos = glm::unProject(screenX, view, projection, viewPort);

        printf("Trying to plant model on position [%f,%f,%f]\n", pos.x, pos.y, pos.z);

        App* app = App::GetInstance();

        if (app->model != nullptr && app->shader != nullptr && app->material != nullptr && app->texture != nullptr)
        {
            app->scene->AddModel(new DrawableObject(app->model, app->shader, app->material, app->texture, new TransformationTranslate(pos)));
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

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
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
    this->shader = lambert;

    //Attach shader to camera
    this->camera = new Camera();
    camera->AttachObserver(constant);
    camera->AttachObserver(lambert);
    camera->AttachObserver(phong);
    camera->AttachObserver(blinn);
    camera->NotifyObservers();

    //Create scene
    this->scene = new Scene();

#define SCENE 3

#if SCENE == 1
    //Four balls
    
    //Create light
    PositionedLight* light = new PositionedLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0, 0.0, 0.0));
    light->AttachObserver(lambert);
    light->AttachObserver(phong);
    light->AttachObserver(blinn);
    light->NotifyObservers();

    //Create material
    Material* material = new Material(glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.8, 0.8, 0.8), glm::vec3(1.0, 1.0, 1.0), 32);

    //Create texture
    Texture* texture = new Texture("models/earth.jpg");

    //Create models
    TransformationTranslate* transformation1 = new TransformationTranslate(glm::vec3(0.5f, 0.0f, 0.0f));
    TransformationTranslate* transformation2 = new TransformationTranslate(glm::vec3(-0.5f, 0.0f, 0.0f));
    TransformationTranslate* transformation3 = new TransformationTranslate(glm::vec3(0.0f, 0.0f, 0.5f));
    TransformationTranslate* transformation4 = new TransformationTranslate(glm::vec3(0.0f, 0.0f, -0.5f));
    Model* sphere = new Model("models/sphere.obj");
    scene->AddModel(new DrawableObject(sphere, phong, material, texture, transformation1));
    scene->AddModel(new DrawableObject(sphere, phong, material, texture, transformation2));
    scene->AddModel(new DrawableObject(sphere, phong, material, texture, transformation3));
    scene->AddModel(new DrawableObject(sphere, phong, material, texture, transformation4));

#elif SCENE == 2
    //Solar system

    //Create ligh´t
    PositionedLight* light = new PositionedLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0, 0.0, 0.0));
    light->AttachObserver(lambert);
    light->AttachObserver(phong);
    light->AttachObserver(blinn);
    light->NotifyObservers();

    //Create material
    Material* material = new Material(glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.8, 0.8, 0.8), glm::vec3(1.0, 1.0, 1.0), 32);

    //Create texture
    Texture* sunTexture = new Texture("models/sun.jpg");
    Texture* mercuryTexture = new Texture("models/mercury.jpg");
    Texture* venusTexture = new Texture("models/venus.jpg");
    Texture* earthTexture = new Texture("models/earth.jpg");
    Texture* marsTexture = new Texture("models/mars.jpg");
    Texture* jupiterTexture = new Texture("models/jupiter.jpg");
    Texture* saturnTexture = new Texture("models/saturn.jpg");
    Texture* uranusTexture = new Texture("models/uranus.jpg");
    Texture* neptuneTexture = new Texture("models/neptune.jpg");
    Texture* plutoTexture = new Texture("models/pluto.jpg");

    //Create models
    Model* sphere = new Model("models/sphere.obj");
    scene->AddModel(new DrawableObject(sphere, constant, material, sunTexture, new TransformationScale(glm::vec3(3.0f))));
    scene->AddModel(new DrawableObject(sphere, phong, material, mercuryTexture, new TransformationScale(glm::vec3(0.5f)), new CircularMovement(0, 360, 0, 1.1421, 1, glm::vec3(0.0f), 0.5)));
    scene->AddModel(new DrawableObject(sphere, phong, material, venusTexture, new TransformationScale(glm::vec3(0.9f)), new CircularMovement(0, 360, 0, 0.8305, 1, glm::vec3(0.0f), 1)));
    scene->AddModel(new DrawableObject(sphere, phong, material, earthTexture, new TransformationScale(glm::vec3(1.0f)), new CircularMovement(0, 360, 0, 0.7112, 1, glm::vec3(0.0f), 1.5)));
    scene->AddModel(new DrawableObject(sphere, phong, material, marsTexture, new TransformationScale(glm::vec3(0.7f)), new CircularMovement(0, 360, 0, 0.5736, 1, glm::vec3(0.0f), 2.5)));
    scene->AddModel(new DrawableObject(sphere, phong, material, jupiterTexture, new TransformationScale(glm::vec3(1.5f)), new CircularMovement(0, 360, 0, 0.3100, 1, glm::vec3(0.0f), 3.5)));
    scene->AddModel(new DrawableObject(sphere, phong, material, saturnTexture, new TransformationScale(glm::vec3(1.4f)), new CircularMovement(0, 360, 0, 0.2292, 1, glm::vec3(0.0f), 4.5)));
    scene->AddModel(new DrawableObject(sphere, phong, material, uranusTexture, new TransformationScale(glm::vec3(0.8f)), new CircularMovement(0, 360, 0, 0.1605, 1, glm::vec3(0.0f), 5.5)));
    scene->AddModel(new DrawableObject(sphere, phong, material, neptuneTexture, new TransformationScale(glm::vec3(0.8f)), new CircularMovement(0, 360, 0, 0.1319, 1, glm::vec3(0.0f), 6.5)));
    scene->AddModel(new DrawableObject(sphere, phong, material, plutoTexture, new TransformationScale(glm::vec3(0.3f)), new CircularMovement(0, 360, 0, 0.1146, 1, glm::vec3(0.0f), 8)));
    TransformationComposite* moonTransformation = new TransformationComposite();
    TransformationRotate* moonRotation = new TransformationRotate(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    moonTransformation->AddTransformation(moonRotation);
    moonTransformation->AddTransformation(new TransformationTranslate(glm::vec3(0.0f, 0.0f, 0.25f)));
    moonTransformation->AddTransformation(new TransformationScale(glm::vec3(0.25, 0.25, 0.25)));
    scene->AddModel(new DrawableObject(sphere, phong, material, mercuryTexture, moonTransformation, new CircularMovement(0, 360, 0, 0.7112, 1, glm::vec3(0.0f), 1.5)));

#elif SCENE == 3
    //Create lights
    PositionedLight* light = new PositionedLight(glm::vec4(1.0f, 1.0f, 0.7f, 1.0f), glm::vec3(0.0, 1.0, 15.0));
    light->AttachObserver(lambert);
    light->AttachObserver(phong);
    light->AttachObserver(blinn);
    light->NotifyObservers();
    ReflectorLight* light2 = new ReflectorLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.15f, 0.15f, glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.0, -1.0, 0.0), 15.0f);
    light2->AttachObserver(lambert);
    light2->AttachObserver(phong);
    light2->AttachObserver(blinn);
    light2->NotifyObservers();
    this->flashLight = light2;
    DirectionLight* light3 = new DirectionLight(glm::vec4(0.15f, 0.15f, 0.15f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    light3->AttachObserver(lambert);
    light3->AttachObserver(phong);
    light3->AttachObserver(blinn);
    light3->NotifyObservers();

    //Create material
    Material* material = new Material(glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.8, 0.8, 0.8), glm::vec3(1.0, 1.0, 1.0), 32);
    this->material = material;

    //Create skybox
    TransformationTranslate* skyboxMovement = new TransformationTranslate(glm::vec3(0.0, 0.0, 0.0));
    scene->SetSkybox(new DrawableObject(new Model("models/skybox.obj"), constant, material, new Texture("models/skybox_dark.jpg"), skyboxMovement));
    this->skyboxMovement = skyboxMovement;

    //Create models
    Model* model = new Model("models/LowPolyTreePack.obj");
    this->model = model;
    Texture* texture = new Texture("models/LowPolyTreePack.png");
    this->texture = texture;

    scene->AddModel(new DrawableObject(new Model("models/plane.obj"), lambert, material, new Texture("models/grass.jpg"), new TransformationScale(glm::vec3(200.0, 1.0, 200.0))));
    scene->AddModel(new DrawableObject(new Model("models/model.obj"), phong, material, new Texture("models/test.jpg")));
    Movement* movement = new LinearMovement(0, 1, 0, 0.01, 0, glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(3.0f, 0.0f, 0.0f));
    scene->AddModel(new DrawableObject(new Model("models/zombie.obj"), lambert, material, new Texture("models/zombie.png"), new TransformationTranslate(glm::vec3(0.0, 0.0, 12.0)), movement));

    srand(time(nullptr));
    for (int i = 0; i < 200; i++)
    {
        float x = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (100))));
        if(rand()%2 == 0)
			x *= -1;
        float z = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (100))));
        if (rand() % 2 == 0)
            z *= -1;
        TransformationComposite* treeTransformation = new TransformationComposite();
        treeTransformation->AddTransformation(new TransformationTranslate(glm::vec3(x, -1.0f, z)));
        float scale = 0.75 + static_cast <float> (rand() / (static_cast <float> (RAND_MAX)));
        treeTransformation->AddTransformation(new TransformationScale(glm::vec3(scale, scale, scale)));
        float rotation = static_cast <float> (rand() / (static_cast <float> (RAND_MAX / 360)));
        treeTransformation->AddTransformation(new TransformationRotate(glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)));
        scene->AddModel(new DrawableObject(model, lambert, material, texture, treeTransformation));
    }

#endif
       
    //Run Infinite Loop
    while (!glfwWindowShouldClose(window)) {
        // clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        // render scene
        scene->Render();
#if SCENE == 2
        moonRotation->angle += 0.1;
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
