
#pragma once
// Bullet needs some basic info

#include <bullet/btBulletDynamicsCommon.h>
#include "PhysicsDraw.h"

// this is a support class to get return data to the game status, it won't have much extra info, so its safe in here.
//The only thing we really want is the worldTransform of the MotionState which supplies us with position and orientation
// data to allow us to render objects 
class GameValues : public btDefaultMotionState
{
public:
	GameValues(const btTransform& transform) // set up with our transform matrix
		: btDefaultMotionState(transform) {	}

	void GetWorldTransform(btScalar* transform)
	{
		btTransform l_transform; // create a local bullet transform
		getWorldTransform(l_transform); // load it with the World Transform for this object
		l_transform.getOpenGLMatrix(transform); // convert to OpenGL format and store in the address provided
	}
};


class PhysicsObj
{
public:
	PhysicsObj(btCollisionShape* Shape,  // to create a new object it needs shape 
		float mass,						// and mass data 
		const btVector3& Position, // these are defaults, but best to be replaces
		const btQuaternion& Rotation);  // here be dragons...but glm and bt, can handle it

	~PhysicsObj();

	// best to allow our Object to set and return use things in accessors	
	inline btCollisionShape* GetShape() { return m_Shape; }
	inline void	SetShape(btCollisionShape* S) { m_Shape = S; }

	inline btRigidBody* GetRigidBody() { return m_Body; }
	inline void SetRighidBody(btRigidBody* RB) { m_Body = RB; }
	inline btMotionState* GetMotionState() { return m_Status; }


	// couple of useful utility functions to simplify the rotate and position of our physics objects 
	// these act on the Rigid body and therefore will reposition/orient an object while it is
	// in the physics world, useful for restarting or teleporting.
	inline void SetOrientation(btQuaternion Rotation)
	{
		// could just m_body->setXXX but there's a slim chance you might add multiple rigid bodies to an object
		// and you would make GetRigidBody return the active one, so this is a safer method to allow for later expansion
		GetRigidBody()->setWorldTransform(btTransform(Rotation, m_Body->getWorldTransform().getOrigin()));
	}

	inline void SetPosition(btVector3 Position)
	{
		GetRigidBody()->setWorldTransform(btTransform(m_Body->getOrientation(), Position));

	}




	// This gets the Motionstate transform, which is useful variables made available after the step
	// as these are generated by the step system, they should be used as read only, as writing to them
	// has no effect and sending them back to the motionstate is not useful,
	void GetTransform(btScalar* transform)
	{
		if (m_Status)
			m_Status->GetWorldTransform(transform); //this is the important part
	}


	bool AmIAlive = false; // only the particle colliders use this as they can reuse the colliders, so we need a means to switch them off and on and indicate their state

protected:

	btRigidBody* m_Body = nullptr;
	btCollisionShape* m_Shape = nullptr;
	GameValues* m_Status; // the general position, orientation and scale of our object to return back

};
