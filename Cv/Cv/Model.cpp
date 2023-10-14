#include "Model.h"

Model::Model(float points[], uint8_t size)
{
    //vertex buffer object (VBO)
    this->VBO = 0;
    glGenBuffers(1, &VBO); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);

    //Vertex Array Object (VAO)
    this->VAO = 0;
    glGenVertexArrays(1, &VAO); //generate the VAO
    glBindVertexArray(VAO); //bind the VAO
    glEnableVertexAttribArray(0); //enable vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    this->verticeCount = size / sizeof(float);
}

void Model::Render()
{
    glBindVertexArray(this->VAO);
    // draw triangle
    glDrawArrays(GL_TRIANGLES, 0, this->verticeCount); //mode,first,count
}