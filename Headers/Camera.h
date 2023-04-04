#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
class Input;
class Camera
{
public:
	Camera();
	Camera(glm::vec3 position);
	~Camera();

	glm::mat4 View;
	glm::vec3 Position, Front, Up, Right;
	glm::mat4 Projection= glm::mat4(1.f); //= glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 200.0f);
	void calculateViewMatrix();
	void calculateCameraVectors();
	void update(Input* TheInput, float deltaTime);

	float yaw = 0;
	float pitch = 0;

	/*glm::vec3 GetPosition() const {
		return Position;
	}
	glm::vec3 GetFront() const {
		return Front;
	}
	glm::vec3 GetUp() const {
		return Up;
	}
	glm::mat4 GetView() const {
		return View;
	}*/
	/*glm::mat4 GetProjection() const {
		return Projection;
	}*/
	/*void GetCamera() const {
		return Camera;
	}*/
	float GetYaw() { return yaw; }
	float GetPitch() { return pitch; }
};

