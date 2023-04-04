#pragma once

#include "btBulletDynamicsCommon.h"
#include "Camera.h"
#include "ObjectModel.h"
#include "Graphics.h"
#include "Input.h"
#include "PhysicsDraw.h"

typedef struct {

	btRigidBody* WhoWeHit = nullptr;
	btVector3 WhereWeHit = btVector3(0, 0, 0);
	float Distance = -1;
	bool WasItStatic = true;

} RayReturn;

class Ray
{
public:
	Ray();
	~Ray();
	glm::vec3 GetAnotherRayFromMouse(float mouseX, float mouseY, Camera* cam, btDiscreteDynamicsWorld* theWorld, PhysicsDraw* physDraw);
	bool Raycast(btDiscreteDynamicsWorld* theWorld, btVector3 &startPosition, btVector3 &direction, PhysicsDraw* physDraw, btRigidBody* selectedObject);

	bool RayHit(btDiscreteDynamicsWorld* theWorld, float x, float y, PhysicsDraw* physDraw, Camera* cam);

	glm::vec3 ScreenSpaceToWorldSpace(glm::vec3 ScreenPos, Camera* cam);
	

public:


	btRigidBody* selectedObject = nullptr;

	glm::vec3 CurrentRay;
	glm::mat4 ViewMatrix;
	glm::mat4 Projection;
	glm::vec2 ScreenSizes;

	btVector3 m_cameraPosition; // the camera's current position
	btVector3 m_cameraTarget;	 // the camera's lookAt target
	float m_nearPlane; // minimum distance the camera will render
	float m_farPlane; // farthest distance the camera will render
	btVector3 m_upVector; // keeps the camera rotated correctly
	float m_cameraDistance; // distance from the camera to its target
	float m_cameraPitch; // pitch of the camera 
	float m_cameraYaw; // yaw of the camera

	int m_screenWidth;
	int m_screenHeight;

	Camera* Cam;
	Input* TheInput;
	PhysicsDraw* physDraw;

	glm::vec2 MouseScreenCoord;
	glm::vec2 NormalizedMouse;

	glm::vec3 MousePlanePoint; // the plane intersection
	glm::vec3 MousePlanePointLevel;

	glm::vec3 lRayStart_world;
	glm::vec3 lRayEnd_world;

	glm::vec3 WhatThePoint = glm::vec3(0.0f);
	btVector3 StartVector;
	glm::vec3 intersectionPoint;

	bool rayHit;
};
