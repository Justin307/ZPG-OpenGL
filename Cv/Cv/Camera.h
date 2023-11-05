#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <vector>
#include "Observable.h"

class Shader;

class Camera : public Observable{
private:
    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 up;
    float alpha;
    float fi;
    float widht;
    float height;
public:
    Camera();
    glm::mat4 GetView();
    glm::mat4 GetProjection();
    glm::vec3 GetPosition();
    glm::vec3 GetDirection();
    void MoveFront();
    void MoveBack();
    void MoveLeft();
    void MoveRight();
    void MoveUp();
    void MoveDown();
    void MoveMouse(float x, float y);
    void SetWidthHeight(float width, float height);
    void NotifyObservers();
};