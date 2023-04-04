#include <glm/gtc/matrix_transform.hpp>
#include "../Headers/Camera.h"
#include "../Headers/Input.h"

#include "../Headers/Graphics.h"
#include "../Headers/ObjectModel.h"

#include "../Headers/CubeModel.h"
#include "../Headers/ModelManager.h"

#include "../Headers/PhysicsDraw.h"
#include "../Headers/PhysicsObject.h"
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>
#include "../Headers/ImGuiManager.h"
#include "../Headers/MousePicker.h"
#include "../Headers/Ray.h"
#include "../Headers/Perlin.h"
#include "../Headers/Terrain.h"
#include <random>

using namespace glm;

// keeping some things in global space...because....Brian does it this way :D you should consider better ways....AKA, you should consider better ways...do I have to spell it out?
std::vector<ObjectModel*> MyObjects; // on the basis that every object is derived from ObjectModel, we keep a list of things to draw.
std::vector<PhysicsObj*> physObj;

PhysicsDraw* physDraw;
Graphics graphics;  // create a 1 time instance of a graphics class in Global space
ImGuiManager* imgui;
HeightMap* heightmap;
ShaderManager* shader;

Target_State state;
Target_State* p_state = &state;
Input* TheInput; // nice pointer to a hardware key and mouse thread system... but..it can be flakey with some wireless keyboards

Gamestates states = Gamestates::Start;

Ray* ray;
MousePicker* mousePicker;
Camera* cam;
btBroadphaseInterface* broadPhase;
btDefaultCollisionConfiguration* collisionConfig;
btCollisionDispatcher* dispatcher;
btSequentialImpulseConstraintSolver* solver;
btDiscreteDynamicsWorld* dynamicWorld;

std::random_device randomG;
std::mt19937 eng(randomG());
std::uniform_int_distribution<> distPos(-15, 10);
std::uniform_int_distribution<> diffName(0, 10);

char* RandomNames()
{
	char* names[] = { "Ana", "Ari", "Mimi" , "Alec" , "Tudi", /*"Dar" ,*/ "Cata", "Dia", "Madi", "Lio", "Pati" };
	int randomIndex = rand() % 11;
	return names[diffName(eng)];
	//std::cout << "Random names = " << RandomNames() << std::endl ;
}

glm::mat4 blScalarToMat4(float sc[]) {
	return glm::mat4{
		sc[0],sc[1],sc[2],sc[3],
		sc[4],sc[5],sc[6],sc[7],
		sc[8],sc[9],sc[10],sc[11],
		sc[12],sc[13],sc[14],sc[15]
	};
}

bool SlerpToTarget(glm::vec3& tp, btRigidBody * body,float DT,float TurningRate) {
	btQuaternion Current = body->getOrientation(); // now lets change orientation?
	// can't do this in bullet
	btVector3 random = body->getCenterOfMassPosition();
	glm::vec3 bodyPos = glm::vec3(random.x(), tp.y, random.z());


	glm::quat orientation = glm::conjugate(glm::quat(glm::lookAt(tp, bodyPos, glm::vec3(0, 1, 0))));
	glm::quat gcurr = glm::quat(Current.getW(), Current.getX(), Current.getY(), Current.getZ());
	glm::quat orientation2 = glm::slerp(gcurr, orientation, DT* TurningRate  ); // need to incorporate some inertia/mass into his turn



	btQuaternion btQuatlook;
	btQuatlook.setX(orientation2.x);
	btQuatlook.setY(orientation2.y);
	btQuatlook.setZ(orientation2.z);
	btQuatlook.setW(orientation2.w);
	body->setWorldTransform(btTransform(btQuatlook, random));//(btQuatlook);
	return true;
}

void Update(Target_State* p_state)
{
	// Setup the viewport
	glViewport(0, 0, p_state->width, p_state->height);

	// Clear the color draw buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	dynamicWorld->stepSimulation(1.0f / 60.0f, 10);

	cam->update(TheInput, 0.5f);


	float matrix[16];
	glm::mat4 model;
	glm::vec2 selectionBoxStart = { std::min(imgui->startpos.x,imgui->endpos.x),std::min(imgui->startpos.y,imgui->endpos.y) };
	glm::vec2 selectBoxEnd = { std::max(imgui->startpos.x,imgui->endpos.x),std::max(imgui->startpos.y,imgui->endpos.y) };

	glm::vec3 ForwardGLM;
	btVector3 intersectionPoint;

	

	for (size_t i = 0; i < MyObjects.size(); i++)
	{
		btQuaternion orient = physObj[i]->GetRigidBody()->getOrientation();
		btVector3 Forward = btVector3(0, 0, 1);
		ForwardGLM = glm::normalize(glm::vec3(orient.x(), orient.y(), orient.z()));
		Forward = quatRotate(orient, Forward);

		physObj[i]->GetRigidBody()->getWorldTransform().getOpenGLMatrix(matrix);
		model = blScalarToMat4(matrix);
		glm::vec4 tankPos(physObj[i]->GetRigidBody()->getCenterOfMassPosition().x(), physObj[i]->GetRigidBody()->getCenterOfMassPosition().y(), physObj[i]->GetRigidBody()->getCenterOfMassPosition().z(), 1.0f);
		tankPos = cam->Projection * cam->View * tankPos;
		tankPos = tankPos / tankPos.w;



		if (imgui->ReturnMouseInput(rightClick))
		{
			if (selectionBoxStart.x <= tankPos.x && selectBoxEnd.x >= tankPos.x && selectionBoxStart.y <= tankPos.y && selectBoxEnd.y >= tankPos.y)
			{
				//imgui->ReturnMouseInput(rightClick);
				MyObjects[i]->selected = 1.f;

					/*if (MyObjects[i]->selected == 0)
				
					{
						MyObjects[i]->selected = 1;
					}*/
					//else
					//{
					//	//MyObjects[i]->selected = 1;

					//}
				
				//if (MyObjects[i]->selected = 0.f)
				//{
				//}
				//else
				//{
				//	if (MyObjects[i]->selected == 1.f)
				//	{
				//		//mousePicker->Update();
				//		//btVector3 intersectionPoint;
				//		//if (ray->RayHit(dynamicWorld, imgui->currentmousepos.x, imgui->currentmousepos.y, physDraw /*intersectionPoint*/)) {
				//		//	// Calculate the direction to the intersection point
				//		//	btVector3 direction = intersectionPoint - physObj[i]->GetRigidBody()->getCenterOfMassPosition();
				//		//	direction.normalize();

				//		//	// Apply a force to move the object towards the intersection point
				//		//	//physObj[i]->GetRigidBody()->applyCentralImpulse(direction * 0.1);
				//		//	physObj[i]->GetRigidBody()->setAngularVelocity(btVector3(0.0f, 0.5f, 0.0f));
				//		//	physObj[i]->GetRigidBody()->applyCentralImpulse(Forward.normalize() * 0.1);
				//		//	//imgui->ReturnMouseInput(leftClick);

				//		//}
				//		//ray->RayHit(dynamicWorld, imgui->currentmousepos.x, imgui->currentmousepos.y, physDraw);

				//	}
				//}
				
			}
			else
			{
				MyObjects[i]->selected = 0.f;
			}
		}
		if (imgui->ReturnMouseInput(leftClick))
		{
			if(MyObjects[i]->selected == 1.f){
				//mousePicker->Update();
				btVector3 intersectionPoint = btVector3 (ray->intersectionPoint.x, ray->intersectionPoint.y, ray->intersectionPoint.z);
				if (ray->rayHit) {

						physObj[i]->GetRigidBody()->activate();
						SlerpToTarget(ray->intersectionPoint, physObj[i]->GetRigidBody(),0.04f,1.f);

						glm::vec3 distVect = (ray->intersectionPoint - glm::vec3(physObj[i]->GetRigidBody()->getCenterOfMassPosition().x(), physObj[i]->GetRigidBody()->getCenterOfMassPosition().y(), physObj[i]->GetRigidBody()->getCenterOfMassPosition().z()));
						float distance = glm::length(distVect);

						cout << "distance->  " << distance << endl;
						cout << "distance->  " << distVect.x << "  " << distVect.y << "  " << distVect.z << endl;
					
						if(distance >= 5.0f)
						physObj[i]->GetRigidBody()->setLinearVelocity(Forward.normalize()* 5.0f /** 0.1*/);

						
					

				}
				//ray->RayHit(dynamicWorld, imgui->currentmousepos.x, imgui->currentmousepos.y, physDraw);
			}
			else
			{
				physObj[i]->GetRigidBody()->setActivationState(ISLAND_SLEEPING);
			}

			//---------- BULLET SEE COMM IN RAY
			//physObj[i]->GetRigidBody()->setUserPointer()
		}
			//physObj[i]->GetRigidBody()->setAngularVelocity(btVector3(0.0f, 0.5f, 0.0f));

			//physObj[i]->GetRigidBody()->applyCentralImpulse(Forward.normalize() * 0.1);
			//physObj[i]->GetRigidBody()->setLinearVelocity(btVector3(front.x, front.y, front.z));

			//update the object, in this demo, we just up its position and transform data but you should consider logic
			MyObjects[i]->Update(cam->View, model);
			// the draw routine is the responsbility of the object itself, thats not an ideal system, consider how to improve
			MyObjects[i]->Draw();
			physDraw->drawLine(glm::vec3(tankPos), glm::vec3(tankPos) + glm::vec3(0, 0.2, 0));
			//physDraw->DoDebugDrawForSelectionBox();
	}

	physDraw->SetView(cam->View);
	physDraw->drawLine(glm::vec3(imgui->startpos.x, imgui->startpos.y, 0.0f), glm::vec3(imgui->endpos.x, imgui->startpos.y, 0.0f));
	physDraw->drawLine(glm::vec3(imgui->startpos.x, imgui->endpos.y, 0.0f), glm::vec3(imgui->endpos.x, imgui->endpos.y, 0.0f));
	physDraw->drawLine(glm::vec3(imgui->endpos.x, imgui->startpos.y, 0.0f), glm::vec3(imgui->endpos.x, imgui->endpos.y, 0.0f));
	physDraw->drawLine(glm::vec3(imgui->startpos.x, imgui->startpos.y, 0.0f), glm::vec3(imgui->startpos.x, imgui->endpos.y, 0.0f));
	physDraw->DoDebugDrawForSelectionBox();
	dynamicWorld->debugDrawWorld();
	physDraw->DoDebugDraw();
}


// Our projects main  Game loop (could be put into a class, hint hint)
void  MainLoop(Target_State* esContext)
{
	const int numTanks = 10;
	ObjectModel* Bullet;   // so both types even though quite different use the same base to create them
	ObjectModel* tanks[numTanks] = { nullptr };
	PhysicsObj* obj;

	//---------- INIT POINTERS
	heightmap = new HeightMap();
	ray = new Ray();
	shader = new ShaderManager();
	TheInput = new Input();
	TheInput->Init();

	cam = new Camera(glm::vec3(0.f, -8.f, 44.f));
	mousePicker = new MousePicker(cam, glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 200.0f));

	imgui = new ImGuiManager(&graphics);
	imgui->Init(TheInput);


	//---------- Terrain

	heightmap->Init();

	switch (states)
	{
	case Gamestates::Start:
		break;
	case Gamestates::Loading:
		break;
	case Gamestates::Game:
		break;
	case Gamestates::SecondScene:
		break;
	case Gamestates::GameOver:
		break;
	default:
		break;
	}


	LoadFiles* Handler = new LoadFiles();

	//----------------- BULLET PHYS 

	broadPhase = new btDbvtBroadphase();
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	solver = new btSequentialImpulseConstraintSolver();
	dynamicWorld = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, collisionConfig);
	physDraw = new  PhysicsDraw();
	dynamicWorld->setGravity(btVector3(0.f, -9.8f, 0.f));
	dynamicWorld->setDebugDrawer(physDraw);

	//------------- THE CUBE MODEL --------> Future bullets for the tanks 

	Bullet = new CubeModel(Handler); // make a new cube
	glm::vec3 Pos = glm::vec3(10.0f, -15.0f, 0.0f); // set a position
	Bullet->Scales = glm::vec3(0.1f, 0.1f, 0.1f);		 // a cube is actually quite large (screen size) so shrink it down
	Bullet->SetPosition(Pos);
	MyObjects.push_back(Bullet); // store in the vector ready for the game loop to process
	Bullet->StoreGraphicClass(&graphics); // make sure it knows the where the graphics class is, (for now it contains our attribute/uniform info)
	graphics.Init(Bullet); 

	obj = new PhysicsObj(new btBoxShape(btVector3(0.25f, 0.25f, 0.25f)), 2, btVector3(10.0f, -15.0f, 0.0f), btQuaternion(0.f, 0.f, 0.f, 1.f));
	physObj.push_back(obj);
	dynamicWorld->addRigidBody(obj->GetRigidBody());

	//------------ TANKS AND DEBUG LINES
	ModelManager* tankModel = new ModelManager((char*)"../../../Resources/Models/Tank.obj", Handler);


	for (int i = 0; i < numTanks; i++) {
		tanks[i] = new ModelManager(*tankModel); // pass the reference of the tankModel instance
		glm::vec3 position = glm::vec3(-8.0f + i * 2, -15.0f, 0.0f + i * 6);
		tanks[i]->Scales = glm::vec3(1.5f, 1.5f, 1.5f);
		tanks[i]->SetPosition(position);
		tanks[i]->StoreGraphicClass(&graphics);
		graphics.Init(tanks[i]);
		MyObjects.push_back(tanks[i]);

		btVector3 force(0.0, 0.0, 100.0);
		//->applyCentralForce(force);
		btConvexHullShape* prot = new btConvexHullShape();//0.25
		glm::vec3 min = tanks[i]->min, max = tanks[i]->max;
		prot->addPoint(btVector3(min.x, min.y, min.z));
		prot->addPoint(btVector3(max.x, min.y, min.z));
		prot->addPoint(btVector3(max.x, min.y, max.z));
		prot->addPoint(btVector3(min.x, min.y, max.z));
		prot->addPoint(btVector3(min.x, max.y, min.z));
		prot->addPoint(btVector3(max.x, max.y, min.z));
		prot->addPoint(btVector3(max.x, max.y, max.z));
		prot->addPoint(btVector3(min.x, max.y, max.z));
		prot->initializePolyhedralFeatures();
		obj = new PhysicsObj(prot, 2.f, btVector3(-8.0f + i * 2, -15.0f, 0.0f + i * 6), btQuaternion(0.f, 0.f, 0.f, 1.f));
		physObj.push_back(obj);
		dynamicWorld->addRigidBody(obj->GetRigidBody());
	}

	//------------ ENEMY TANKS AND DEBUG LINES 
	ModelManager* enemyTankModel = new ModelManager((char*)"../../../Resources/Models/Tank.obj", Handler);


	for (int i = 0; i < numTanks; i++) {
		tanks[i] = new ModelManager(*enemyTankModel); // pass the reference of the tankModel instance
		glm::vec3 position = glm::vec3(-18.0f + i * 2, -15.0f, 0.0f + i * 6);
		tanks[i]->Scales = glm::vec3(2.f, 2.f, 2.f);
		tanks[i]->SetPosition(position);
		tanks[i]->StoreGraphicClass(&graphics);
		graphics.Init(tanks[i]);
		MyObjects.push_back(tanks[i]);

		btVector3 force(0.0, 0.0, 100.0);
		//->applyCentralForce(force);
		btConvexHullShape* prot = new btConvexHullShape();//0.25
		glm::vec3 min = tanks[i]->min, max = tanks[i]->max;
		prot->addPoint(btVector3(min.x, min.y, min.z));
		prot->addPoint(btVector3(max.x, min.y, min.z));
		prot->addPoint(btVector3(max.x, min.y, max.z));
		prot->addPoint(btVector3(min.x, min.y, max.z));
		prot->addPoint(btVector3(min.x, max.y, min.z));
		prot->addPoint(btVector3(max.x, max.y, min.z));
		prot->addPoint(btVector3(max.x, max.y, max.z));
		prot->addPoint(btVector3(min.x, max.y, max.z));
		prot->initializePolyhedralFeatures();
		obj = new PhysicsObj(prot, 2.f, btVector3(-18.0f + i * 2, -15.0f, 0.0f + i * 6), btQuaternion(0.f, 0.f, 0.f, 1.f));
		physObj.push_back(obj);
		dynamicWorld->addRigidBody(obj->GetRigidBody());
	}


	//--------- FLOORING DEBUG LINES

	obj = new PhysicsObj(new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), btScalar(10.f)), 0, btVector3(0.0f, -25.0f, 0.0f), btQuaternion(0.f, 0.f, 0.f, 1.f));
	physObj.push_back(obj);
	dynamicWorld->addRigidBody(obj->GetRigidBody());

	struct timeval t1, t2;
	struct timezone tz;
	float deltatime;
	float totaltime = 0.0f;
	unsigned int frames = 0;

	gettimeofday(&t1, &tz);

	int Counter = 0;
	while (!TheInput->KeyIsPressed(XK_Escape))// for a timed loop check the counter otherwise find another exit condition
	{

		gettimeofday(&t2, &tz);
		deltatime = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 0.0000001f);
		t1 = t2;

		Update(esContext);  // this is where the action happens
		imgui->Update(TheInput, deltatime, cam, states);
		ray->GetAnotherRayFromMouse(imgui->currentmousepos.x, imgui->currentmousepos.y, cam, dynamicWorld, physDraw);
		heightmap->Draw(/*shader,*/ cam);

		switch (states)
		{
		case Gamestates::Start:
			break;
		case Gamestates::Loading:
			mousePicker->Update();
			break;
		case Gamestates::Game:
			break;
		case Gamestates::SecondScene:
			break;
		case Gamestates::GameOver:
			break;
		default:
			break;
		}

		// report frame rate
		totaltime += deltatime;
		frames++;
		if (totaltime > 1.0f)
		{
			printf("%4d frames rendered in %1.4f seconds -> FPS=%3.4f\n", frames, totaltime, frames / totaltime);  // uncomment to see frames
			totaltime -= 1.0f;
			frames = 0;
		}
		glFlush();
		eglSwapBuffers(esContext->display, esContext->surface);

	}


}


//Program Entry

int main(int argc, char* argv[])
{
	LoadFiles* Handler = new LoadFiles(); // here have a nice file handler
	graphics.GetDisplay();
	graphics.esInitContext(p_state);

	graphics.init_ogl(p_state, 1920, 1080, 1920, 1080); // this is the window size we set , you can set it to anything, the 3rd and 4th params are for older pis'
	graphics.GetRes(graphics.ResolutionX, graphics.ResolutionY);

	ObjectData object_data;  // create a single instance of object_data to initially draw all objects with the same data.

	p_state->object_data = &object_data; // this is....inefficient... an expansion of the old hello triangle demo's consider reworking it.

	// now go do the Game loop	
	MainLoop(p_state);

	// here we should do a graceful exit	
	// I said graceful.....
}
