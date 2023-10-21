#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <vector>
#include "Observer.h"

class Shader;

class Camera {
private:
    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 up;
    float alpha;
    float fi;
    float widht;
    float height;
    std::vector<Observer*> observers;
    void NotifyObservers();
public:
    Camera();
    glm::mat4 GetView();
    glm::mat4 GetProjection();
    void MoveFront();
    void MoveBack();
    void MoveLeft();
    void MoveRight();
    void MoveUp();
    void MoveDown();
    void MoveMouse(float x, float y);
    void AttachObserver(Observer* observer);
    void DettachObserver(Observer* observer);
    void SetWidthHeight(float width, float height);
};