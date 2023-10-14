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
    void AttachObserver(Observer* observer);
    void DettachObserver(Observer* observer);
};