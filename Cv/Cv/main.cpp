//JUS0018 - Ondřej Just

//Include GLEW
#include <GL/glew.h>
//Include GLFW
#include <GLFW/glfw3.h> 

//Include the standard C++ headers  
#include <stdlib.h>
#include <stdio.h>

//Include my headers
#include "app.h"


int main()
{
    App* app = App::GetInstance();
    app->run();

    exit(EXIT_SUCCESS);
}