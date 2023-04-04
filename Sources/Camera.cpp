#include "../Headers/Camera.h"
#include <X11/keysym.h>
#include "../Headers/Input.h"
#include "../Headers/precomp.h"

Camera::Camera()
{
	Position = glm::vec3(0.f, 0.f, 0.f);
	Front = glm::vec3(0.f, 0.f, -1.f);
	Up = glm::vec3(0.f, 1.f, 0.f);
	Right = glm::vec3(1.f, 0.f, 0.f);
	Projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 200.0f);
	calculateViewMatrix();
}

Camera::Camera(glm::vec3 position)
{
	Position = position;
	Front = glm::vec3(0.f, 0.f, -1.f);
	Up = glm::vec3(0.f, 1.f, 0.f);
	Right = glm::vec3(1.f, 0.f, 0.f);
	Projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 200.0f);
	calculateViewMatrix();
}

void Camera::calculateViewMatrix()
{
	calculateCameraVectors();
	View = glm::lookAt(Position, Position + Front, Up);

}

void Camera::calculateCameraVectors()
{
	glm::vec3 front;
	front.x = sin(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(yaw));
	front.z = -cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	Front = glm::normalize(front);

	Right = glm::normalize(glm::cross(Front, glm::vec3(0.f, 1.f, 0.f)));
	Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::update(Input* TheInput, float deltaTime)
{
	if (TheInput->KeyIsPressed(XK_W))
	{
		Position += Front * deltaTime;
		calculateViewMatrix();
	}
	if (TheInput->KeyIsPressed(XK_S))
	{
		Position -= Front * deltaTime;
		calculateViewMatrix();
	}
	if (TheInput->KeyIsPressed(XK_D))
	{
		Position += Right * deltaTime;
		calculateViewMatrix();
	}
	if (TheInput->KeyIsPressed(XK_A))
	{
		Position -= Right * deltaTime;
		calculateViewMatrix();
	}
	if (TheInput->KeyIsPressed(XK_Up))
	{
		Position += Up * deltaTime;
		calculateViewMatrix();
	}
	if (TheInput->KeyIsPressed(XK_Down))
	{
		Position -= Up * deltaTime;
		calculateViewMatrix();
	}
	if (TheInput->KeyIsPressed(XK_Q))
	{
		pitch -= 0.5f;
		calculateViewMatrix();
	}
	if (TheInput->KeyIsPressed(XK_E))
	{
		pitch += 0.5f;
		calculateViewMatrix();
	}
	if (TheInput->KeyIsPressed(XK_R))
	{
		yaw += 0.5f;
		if (yaw > 89.f)
			yaw = 89.f;
		calculateViewMatrix();
	}
	if (TheInput->KeyIsPressed(XK_F))
	{
		yaw -= 0.5f;
		if (yaw < -89.f)
			yaw = -89.f;

		calculateViewMatrix();
	}
}
