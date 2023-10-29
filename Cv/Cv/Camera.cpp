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

/*
*	https://youtu.be/AWM4CUfffos?si=hKDdLWjiXjY9RI-K
*/
Camera::Camera()
{
	this->eye = { 0.0f, 0.0f, 0.0f };
	this->center = { 0.1f, 0.0f, -0.1f };
	this->up = { 0.0f, 0.1f, 0.0f };
	this->alpha = 0;
	this->fi = 0;
	this->widht = 800;
	this->height = 600;

}

glm::mat4 Camera::GetView()
{
	return glm::lookAt(eye, eye + center, up);
}

glm::mat4 Camera::GetProjection()
{
	return glm::perspective(glm::radians(60.0f), this->widht / this->height, 0.1f, 100.0f);
}

glm::vec3 Camera::GetPosition()
{
	return this->eye;
}

/*
*	https://lms.vsb.cz/pluginfile.php/2227895/mod_resource/content/1/ZPG_Prednasky_4.pdf
*	Page 11/31
*/

void Camera::MoveFront()
{
	eye += (glm::normalize(center)*0.1f);
	this->NotifyObservers();
}

void Camera::MoveBack()
{
	eye -= (glm::normalize(center)*0.1f);
	this->NotifyObservers();
}

void Camera::MoveLeft()
{
	eye -= (glm::normalize(glm::cross(center, up))*0.1f);
	this->NotifyObservers();
}

void Camera::MoveRight()
{
	eye += (glm::normalize(glm::cross(center, up)) * 0.1f);
	this->NotifyObservers();
}

void Camera::MoveUp()
{
	eye -= (glm::normalize(glm::cross(center,glm::cross(center, up))) * 0.1f);
	this->NotifyObservers();
}

void Camera::MoveDown()
{
	eye += (glm::normalize(glm::cross(center, glm::cross(center, up))) * 0.1f);
	this->NotifyObservers();
}

void Camera::MoveMouse(float x, float y)
{
	this->fi += (x*-0.1);
	this->alpha += (y*0.1);

	if (this->alpha > 89.0f)
		this->alpha = 89.0f;
	if (this->alpha < -89.0f)
		this->alpha = -89.0f;

	glm::vec3 c;
	c.x = cos(glm::radians(this->alpha)) * cos(glm::radians(this->fi));
	c.y = sin(glm::radians(this->alpha));
	c.z = cos(glm::radians(this->alpha)) * sin(glm::radians(this->fi));
	this->center = glm::normalize(c);
	this->NotifyObservers();
}

void Camera::SetWidthHeight(float width, float height)
{
	this->widht = width;
	this->height = height;
	this->NotifyObservers();
}

void Camera::NotifyObservers()
{
	for (auto o : observers)
	{
		o->Update(CAMERA, this);
	}
}
