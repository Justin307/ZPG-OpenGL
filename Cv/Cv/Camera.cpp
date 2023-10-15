#include "Camera.h"
#include "Shader.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

/*
* By convention, OpenGL is a right-handed system.
* What this basically says is that the positive x-axis is to your right,
* the positive y-axis is up and the positive z-axis is backwards.
* Think of your screen being the center of the 3 axes
* and the positive z-axis going through your screen towards you.
*/

void Camera::NotifyObservers()
{
	for (auto& x : this->observers)
		x->Update();
}

Camera::Camera()
{
	this->eye = { 0.0f, 0.0f, 0.0f };
	this->center = { 0.1f, 0.0f, -0.1f };
	this->up = { 0.0f, -0.1f, 0.0f };
}

glm::mat4 Camera::GetView()
{
	return glm::lookAt(eye, eye + center, up);
}

glm::mat4 Camera::GetProjection()
{
	return glm::perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
}

/*
*	https://lms.vsb.cz/pluginfile.php/2227895/mod_resource/content/1/ZPG_Prednasky_4.pdf
*	Page 11/31
*/

void Camera::MoveFront()
{
	eye += (glm::normalize(center));
	this->NotifyObservers();
}

void Camera::MoveBack()
{
	eye -= (glm::normalize(center));
	this->NotifyObservers();
}

void Camera::MoveLeft()
{
	eye += (glm::normalize(glm::cross(center, up)));
	this->NotifyObservers();
}

void Camera::MoveRight()
{
	eye -= (glm::normalize(glm::cross(center, up)));
	this->NotifyObservers();
}

void Camera::AttachObserver(Observer* observer)
{
	this->observers.push_back(observer);
}

void Camera::DettachObserver(Observer* observer)
{
	observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}