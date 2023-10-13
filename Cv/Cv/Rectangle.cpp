#include "Rectangle.h"


Rectangle::Rectangle(float points[], uint8_t size)
{
    //vertex buffer object (VBO)
    glGenBuffers(1, &VBO); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);

    //Vertex Array Object (VAO)
    glGenVertexArrays(1, &VAO); //generate the VAO
    glBindVertexArray(VAO); //bind the VAO
    glEnableVertexAttribArray(0); //enable vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

Rectangle::Rectangle(float points[], uint8_t size, Shader* shader)
{
    //vertex buffer object (VBO)
    glGenBuffers(1, &VBO); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);

    //Vertex Array Object (VAO)
    glGenVertexArrays(1, &VAO); //generate the VAO
    glBindVertexArray(VAO); //bind the VAO
    glEnableVertexAttribArray(0); //enable vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    this->shader = shader;
}

Rectangle::Rectangle(float points[], uint8_t size, Shader* shader, TransformationComponent* transformation)
{
    //vertex buffer object (VBO)
    glGenBuffers(1, &VBO); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);

    //Vertex Array Object (VAO)
    glGenVertexArrays(1, &VAO); //generate the VAO
    glBindVertexArray(VAO); //bind the VAO
    glEnableVertexAttribArray(0); //enable vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    this->shader = shader;

    this->transformation = transformation;
}

void Rectangle::Render()
{
    shader->Use();
    if (transformation != nullptr)
    {
        glm::mat4 matrix = glm::mat4(1.0f);
        this->transformation->ApplyTransformation(matrix);
        shader->SetUniformLocationValue(std::string("modelMatrix"), matrix);
    }
    glBindVertexArray(this->VAO);
    // draw triangle
    glDrawArrays(GL_TRIANGLES, 0, 6); //mode,first,count
}

void Rectangle::Render(TransformationComponent* transformation, std::string location)
{
    shader->Use();
    glm::mat4 matrix = glm::mat4(1.0f);
    transformation->ApplyTransformation(matrix);
    shader->SetUniformLocationValue(std::string("modelMatrix"), matrix);
    shader->SetUniformLocationValue(location, matrix);
    glBindVertexArray(this->VAO);
    // draw triangle
    glDrawArrays(GL_TRIANGLES, 0, 6); //mode,first,count
}
