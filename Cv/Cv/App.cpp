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

void App::error_callback(int error, const char* description) { fputs(description, stderr); }

const char* vertex_shader =
"#version 330\n"
"layout(location=0) in vec3 vp;"
"uniform mat4 modelMatrix;"
"void main () {"
"     gl_Position = modelMatrix * vec4 (vp, 1.0);"
"}";

const char* fragment_shader =
"#version 330\n"
"out vec4 frag_colour;"
"void main () {"
"     frag_colour = vec4 (0.5, 0.5, 0.0, 1.0);"
"}";

const char* vertex_shader2 =
"#version 330\n"
"layout(location=0) in vec3 vp;"
"void main () {"
"     gl_Position = vec4 (vp, 1.0);" 
"}";

const char* fragment_shader2 =
"#version 330\n"
"out vec4 frag_colour;"
"void main () {"
"     frag_colour = vec4 (0.0, 0.5, 0.5, 1.0);"
"}";

float points[] = {
     0.0f, 0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};


float points3[] = {
            0.0f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,

            0.0f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, -0.5f, 0.0f,
};

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
}

void App::run()
{
    //GLEW
    printGLEWInfo();

    TransformationComposite rotation;
    rotation.AddTransformation(new TransformationRotate(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    TransformationComposite translation;
    translation.AddTransformation(new TransformationTranslate(glm::vec3(-0.1f, -0.1f, -0.1f)));
    TransformationComposite *transformation = new TransformationComposite();
    transformation->AddTransformation(&rotation);
    transformation->AddTransformation(&translation);
    transformation->AddTransformation(new TransformationScale(glm::vec3(0.5f)));

    //Create shader program
    Shader* shaderWithMatrix = new Shader(vertex_shader, fragment_shader);
    Shader* shaderWithoutMatrix = new Shader(vertex_shader2, fragment_shader2);

    //Create triangle
    Model* tri = new Model(points, sizeof(points));
    Model* quatro = new Model(points3, sizeof(points3));

    //Create scene
    Scene* scene = new Scene();
    scene->AddModel(new DrawableObject(tri, shaderWithoutMatrix));
    scene->AddModel(new DrawableObject(quatro, shaderWithMatrix, transformation));

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