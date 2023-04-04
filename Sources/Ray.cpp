#include "../Headers/Ray.h"
#include "../Headers/Graphics.h"
#include "PhysicsDraw.h"

Ray::Ray()
{
}

Ray::~Ray()
{
}

glm::vec3 Ray::GetAnotherRayFromMouse(float mouseX,float mouseY, Camera*cam, btDiscreteDynamicsWorld* theWorld, PhysicsDraw* physDraw)
{
	//// The ray Start and End positions, in Normalized Device Coordinates(Have you read Tutorial 4 ? )
	//uint32_t ResolutionX = 1920;
	//uint32_t ResolutionY = 1080;
	//
	//glm::vec4 lRayStart_NDC(
	//	((float)mouseX / (float)ResolutionX - 0.5f) * 2.0f, // [0,1920] -> [-1,1]
	//	-(((float)mouseY / (float)ResolutionY - 0.5f) * 2.0f), // [0, 1080] -> [-1,1]
	//	-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
	//	1.0f
	//);
	//glm::vec4 lRayEnd_NDC(
	//	((float)mouseX / (float)ResolutionX - 0.5f) * 2.0f,
	//	-(((float)mouseY / (float)ResolutionY - 0.5f) * 2.0f),
	//	0.0,
	//	1.0f
	//);


	//// The Projection matrix goes from Camera Space to NDC.
	//	// So inverse(ProjectionMatrix) goes from NDC to Camera Space.
	//glm::mat4 InverseProjectionMatrix = glm::inverse(cam->Projection);

	//// The View Matrix goes from World Space to Camera Space.
	//// So inverse(ViewMatrix) goes from Camera Space to World Space.
	//glm::mat4 InverseViewMatrix = glm::inverse(cam->View);

	//glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera /= lRayStart_camera.w;
	//lRayStart_world = InverseViewMatrix * lRayStart_camera; lRayStart_world /= lRayStart_world.w;
	//glm::vec4 lRayEnd_camera = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera /= lRayEnd_camera.w;
	//lRayEnd_world = InverseViewMatrix * lRayEnd_camera;   lRayEnd_world /= lRayEnd_world.w;
	lRayStart_world = ScreenSpaceToWorldSpace(glm::vec3(mouseX, mouseY, -1.0f), cam);
	lRayEnd_world = ScreenSpaceToWorldSpace(glm::vec3(mouseX, mouseY, 0.f), cam);
	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
	lRayDir_world = glm::normalize(lRayDir_world);

	// before we return lets get the world coord of the cursor on the Y0 plane

	float dist;

	btVector3 start(lRayStart_world.x, lRayStart_world.y, lRayStart_world.z);
	btVector3 direction(lRayDir_world.x, lRayDir_world.y, lRayDir_world.z);

	rayHit = Raycast(theWorld, start, direction, physDraw,selectedObject);

	return lRayDir_world;
}


bool Ray::Raycast(btDiscreteDynamicsWorld* theWorld, btVector3& startPosition, btVector3& direction,PhysicsDraw* physDraw, btRigidBody* selectedObject)
{
    btVector3 to = startPosition + direction * 100.0f;
    btVector3 start = startPosition;

    btCollisionWorld::ClosestRayResultCallback rayCallback(start, to);
	//btVector3 
   
    rayCallback.m_collisionFilterMask = 2;
    theWorld->rayTest(start, to, rayCallback);
	
    if (rayCallback.hasHit())
    {	
		//-----tank collision detection
	
		//static_cast < tankmodel*>( rayCallback.m_collisionObject->getUserPointer());

        physDraw->drawLine(rayCallback.m_hitPointWorld, rayCallback.m_hitPointWorld + rayCallback.m_hitNormalWorld * 5,btVector3(0.6f,0.f,0.f));
		physDraw->DoDebugDraw();
		
		intersectionPoint =glm::vec3( rayCallback.m_hitPointWorld.x(), rayCallback.m_hitPointWorld.y(), rayCallback.m_hitPointWorld.z());

		if (selectedObject != nullptr)
		{
			btTransform transform = selectedObject->getWorldTransform();
			transform.setOrigin(rayCallback.m_hitPointWorld);
			selectedObject->setWorldTransform(transform);
		}

        return true;
    }

    return false;
}
bool Ray::RayHit(btDiscreteDynamicsWorld* theWorld, float x, float y, PhysicsDraw* physDraw,Camera* cam)
{

	glm::vec3 rayStart = ScreenSpaceToWorldSpace(glm::vec3(x, y, -1.0f), cam); 
	glm::vec3 rayEnd = ScreenSpaceToWorldSpace(glm::vec3(x, y, 0.0f), cam);

	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
	lRayDir_world = glm::normalize(lRayDir_world);
	
	btVector3 start(rayStart.x, rayStart.y, rayStart.z);
	btVector3 direction(lRayDir_world.x, lRayDir_world.y, lRayDir_world.z);

	btVector3 to = start+ direction * 100.0f;
	//btVector3 start = start;

	btCollisionWorld::ClosestRayResultCallback result(start, to);

	result.m_collisionFilterMask = 2;
	theWorld->rayTest(start, to, result);

	if (result.hasHit()) {
		intersectionPoint = glm::vec3(result.m_hitPointWorld.x(), result.m_hitPointWorld.y(), result.m_hitPointWorld.z());
		physDraw->drawRayLine(intersectionPoint, intersectionPoint + glm::vec3(result.m_hitNormalWorld.x(), result.m_hitNormalWorld.y(), result.m_hitNormalWorld.z()) * 5.0f, glm::vec3(0.6f, 0.f, 0.f));
		physDraw->DoDebugDraw();
		return true;
	}

	return false;
}

glm::vec3 Ray::ScreenSpaceToWorldSpace(glm::vec3 ScreenPos, Camera* cam)
{
	uint32_t ResolutionX = 1920;
	uint32_t ResolutionY = 1080;

	glm::vec4 ScreenPos_NDC(
		((float)ScreenPos.x / (float)ResolutionX - 0.5f) * 2.0f, // [0,1920] -> [-1,1]
		-(((float)ScreenPos.y / (float)ResolutionY - 0.5f) * 2.0f), // [0, 1080] -> [-1,1]
		ScreenPos.z, // The near plane maps to Z=-1 in Normalized Device Coordinates
		1.0f
	);

	// The Projection matrix goes from Camera Space to NDC.
		// So inverse(ProjectionMatrix) goes from NDC to Camera Space.
	glm::mat4 InverseProjectionMatrix = glm::inverse(cam->Projection);

	// The View Matrix goes from World Space to Camera Space.
	// So inverse(ViewMatrix) goes from Camera Space to World Space.
	glm::mat4 InverseViewMatrix = glm::inverse(cam->View);

	glm::vec4 lRayStart_camera = InverseProjectionMatrix * ScreenPos_NDC;    lRayStart_camera /= lRayStart_camera.w;
	glm::vec4 lRayScreen_world = InverseViewMatrix * lRayStart_camera; lRayScreen_world /= lRayScreen_world.w;
	

	
	return glm::vec3(lRayScreen_world);
	


}
	//btVector3 from = btVector3(cam->Position.x, cam->Position.y, cam->Position.z);
	//btVector3 to = btVector3(cursor_x, cursor_y, 1.0f);
	//btCollisionWorld::ClosestRayResultCallback rayCallback(from, to);
	//theWorld->rayTest(from, to, rayCallback);

	//if (rayCallback.hasHit()) {
	//	// An object was hit by the ray, get its position and apply a force towards the intersection point
	//	btRigidBody* body = btRigidBody::upcast(rayCallback.m_collisionObject);
	//	if (body != NULL) {
	//		btVector3 intersectionPoint = rayCallback.m_hitPointWorld;
	//		btVector3 forceDirection = (intersectionPoint - body->getCenterOfMassPosition()).normalize();
	//		body->applyCentralImpulse(forceDirection * forceMagnitude);
	//	}
	//}


//void Ray::Update() {
//	if (Input.GetMouseButtonDown(0)) {
//		ray = camera.ScreenPointToRay(Input.mousePosition);
//
//		createBullet = Instantiate(bullet, camera.transform.position, bullet.transform.rotation);
//		createBullet.AddComponent<Rigidbody>();
//		createBullet.GetComponent<Rigidbody>().AddRelativeForce(new Vector3(0, 1500, 0));
//		createBullet.GetComponent<Rigidbody>().useGravity = false;
//		collider = createBullet.GetComponent<Collider>();
//		Destroy(collider);
//
//		if (Physics.Raycast(ray, out hit)) {
//
//		}
//	}
//	Debug.DrawLine(ray.origin, hit.point, Color.red);
//}