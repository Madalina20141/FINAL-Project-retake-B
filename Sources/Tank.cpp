#include "../Headers/Tank.h"

//Tank::Tank(glm::vec3 position, glm::vec3 rotation, float nmovementSpeed, ModelManager* nmodel, int nteam)
//	: GameObject(position, rotation, nmovementSpeed)
//{
//	m_team = nteam;
//	m_model = nmodel;
//
//	if (m_team == ENEMY)
//		m_hueChange = glm::vec4(1.f, 0.2f, 0.5f, 1.f);
//	m_compoundShape = new btCompoundShape();
//	btConvexHullShape* convexH = dynamic_cast<btConvexHullShape*>(m_model->GetMeshes()[0].GetPhyObject()->GetShape());
//	p_phyObject = ObjectManager::Get().GetBulletWorld()->CreatePhyObject(convexH, 150.f, btVector3(position.x, position.y, position.z), btQuaternion(rotation.x, rotation.y, rotation.z, 1));
//	ObjectManager::Get().GetBulletWorld()->AddPhyObject(p_phyObject, false, m_team);
//	m_vehicleRayCaster = new btDefaultVehicleRaycaster(ObjectManager::Get().GetBulletWorld()->GetDynamicWorld());
//	m_vehicle = new btRaycastVehicle(m_tuning, p_phyObject->GetRigidBody(), m_vehicleRayCaster);
//	// 
//	//
//
//	p_phyObject->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
//
//	//object1->setCollisionFlags(object1CollisionFlags & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
//
//	//m_vehicle->getRigidBody()->getBroadphaseHandle()->m_collisionFilterMask = ~1;
//
//	ObjectManager::Get().GetBulletWorld()->GetDynamicWorld()->addVehicle(m_vehicle);
//	bool isFrontWheel = true;
//
//	////choose coordinate system
//	m_vehicle->setCoordinateSystem(0, 1, 2);
//
//	float connectionHeight = 0.f;
//
//	btTransform transform;
//	btTransform identity;
//	identity.setIdentity();
//	btVector3 mini;
//	btVector3 maxi;
//
//	m_compoundShape->getAabb(transform, mini, maxi);
//
//	mini = btVector3(5.6f, 7.f, 2.16f);
//	p_phyObject->GetMotionState()->getWorldTransform(transform);
//	identity.setIdentity();
//	identity.setOrigin(btVector3(position.x, position.y + 2.f, position.z));
//	m_gVehicleSteering = 0.f;
//	std::cout << "TANK ID POS: " << identity.getOrigin().getX() << " " << identity.getOrigin().getY() << " " << identity.getOrigin().getZ() << "\n";
//
//	p_phyObject->GetRigidBody()->setCenterOfMassTransform(identity);
//	p_phyObject->GetRigidBody()->setLinearVelocity(btVector3(0, 0, 0));
//	p_phyObject->GetRigidBody()->setAngularVelocity(btVector3(0, 0, 0));
//	ObjectManager::Get().GetBulletWorld()->GetDynamicWorld()->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(p_phyObject->GetRigidBody()->getBroadphaseHandle(), ObjectManager::Get().GetBulletWorld()->GetDynamicWorld()->getDispatcher());
//	if (m_vehicle)
//	{
//		m_vehicle->resetSuspension();
//		for (int i = 0; i < m_vehicle->getNumWheels(); i++)
//		{
//			//synchronize the wheels with the (interpolated) chassis worldtransform
//			m_vehicle->updateWheelTransform(i, true);
//		}
//	}
//
//	//m_vehicle->getRigidBody()->setAngularVelocity(btVector3(0.f, 1.f, 0.f));
//
//	btVector3 connectionPointCS0(mini.getX() / 3.75f - (0.3f * m_wheelWidth), connectionHeight, mini.getZ() - m_wheelRadius);
//
//	m_vehicle->addWheel(connectionPointCS0, m_wheelDirectionCS0, m_wheelAxleCS, m_suspensionRestLength, m_wheelRadius, m_tuning, isFrontWheel);
//
//	glm::vec3 wposition = position + glm::vec3(connectionPointCS0.getX(), connectionPointCS0.getY(), connectionPointCS0.getZ());
//
//	//Cube* wheel = new Cube(wheelRadius, 0.5f, wposition , ObjectManager::Get().GetBulletWorld(), ShaderManager::Get().GetModelProgram(), 0);
//	//
//	//wheelsCube.push_back(wheel);
//	//wheelsCube[0]->GetPhyObject()->GetRigidBody()->setGravity(btVector3(0.f, 0.f, 0.f));
//
//	connectionPointCS0 = btVector3(-mini.getX() / 3.75f + (0.3f * m_wheelWidth), connectionHeight, mini.getZ() - m_wheelRadius);
//
//	wposition = position + glm::vec3(connectionPointCS0.getX(), connectionPointCS0.getY(), connectionPointCS0.getZ());
//
//	//wheel = new Cube(wheelRadius, 0.5f, wposition, ObjectManager::Get().GetBulletWorld(), ShaderManager::Get().GetModelProgram(), 0);
//
//	//wheelsCube.push_back(wheel);
//	//wheelsCube[1]->GetPhyObject()->GetRigidBody()->setGravity(btVector3(0.f, 0.f, 0.f));
//
//	m_vehicle->addWheel(connectionPointCS0, m_wheelDirectionCS0, m_wheelAxleCS, m_suspensionRestLength, m_wheelRadius, m_tuning, isFrontWheel);
//	//
//	isFrontWheel = false;
//
//	connectionPointCS0 = btVector3(-mini.getX() / 3.75f + (0.3f * m_wheelWidth), connectionHeight, -mini.getZ() + m_wheelRadius);
//	//
//	wposition = position + glm::vec3(connectionPointCS0.getX(), connectionPointCS0.getY(), connectionPointCS0.getZ());
//	//
//	//wheel = new Cube(wheelRadius, 0.5f, wposition, ObjectManager::Get().GetBulletWorld(), ShaderManager::Get().GetModelProgram(), 0);
//	////
//	//wheelsCube.push_back(wheel);
//	//wheelsCube[2]->GetPhyObject()->GetRigidBody()->setGravity(btVector3(0.f, 0.f, 0.f));
//
//	m_vehicle->addWheel(connectionPointCS0, m_wheelDirectionCS0, m_wheelAxleCS, m_suspensionRestLength, m_wheelRadius, m_tuning, isFrontWheel);
//	//
//	connectionPointCS0 = btVector3(mini.getX() / 3.75f - (0.3f * m_wheelWidth), connectionHeight, -mini.getZ() + m_wheelRadius);
//
//	wposition = position + glm::vec3(connectionPointCS0.getX(), connectionPointCS0.getY(), connectionPointCS0.getZ());
//
//	//wheel = new Cube(wheelRadius, 0.5f, wposition, ObjectManager::Get().GetBulletWorld(), ShaderManager::Get().GetModelProgram(), 0);
//
//	//wheelsCube.push_back(wheel);
//	//wheelsCube[3]->GetPhyObject()->GetRigidBody()->setGravity(btVector3(0.f, 0.f, 0.f));
//
//	m_vehicle->addWheel(connectionPointCS0, m_wheelDirectionCS0, m_wheelAxleCS, m_suspensionRestLength, m_wheelRadius, m_tuning, isFrontWheel);
//
//	for (int i = 0; i < m_vehicle->getNumWheels(); i++)
//	{
//		btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
//		wheel.m_suspensionStiffness = m_suspensionStiffness;
//		wheel.m_wheelsDampingRelaxation = m_suspensionDamping;
//		wheel.m_wheelsDampingCompression = m_suspensionCompression;
//		wheel.m_frictionSlip = m_wheelFriction;
//		wheel.m_rollInfluence = m_rollInfluence;
//	}
//	btTransform transformdebug;
//	p_phyObject->GetMotionState()->getWorldTransform(transformdebug);
//	//std::cout << "TANK POS: " << transformdebug.getOrigin().getX() << " " << transformdebug.getOrigin().getY() << " " << transformdebug.getOrigin().getZ() << "\n";
//}
//
//void Tank::Update(float deltaTime)
//{
//	//CheckSteer();
//
//	m_currentSteerTimer += deltaTime;
//	//if(hasDestination && currentSteerTimer > recalculateSteerTimer)
//	//	FindSteeringDirection(), currentSteerTimer = 0.f;
//	btTransform childTransform;
//	p_phyObject->GetMotionState()->getWorldTransform(childTransform);//m_compoundShape->getChildTransform(i);
//	//std::cout << "TANK LOCATION: " << childTransform.getOrigin().getX() << " " << childTransform.getOrigin().getY() << " " << childTransform.getOrigin().getZ() << "\n";
//
//	//if(isSelected)
//	//	std::cout << "IS SELECTED: " << isSelected << "\n";
//	//btScalar m[16];
//	int i;
//
//
//	btVector3 wheelColor(1, 0, 0);
//
//	btVector3	worldBoundsMin, worldBoundsMax;
//	ObjectManager::Get().GetBulletWorld()->GetDynamicWorld()->getBroadphase()->getBroadphaseAabb(worldBoundsMin, worldBoundsMax);
//
//
//
//	for (i = 0; i < m_vehicle->getNumWheels(); i++)
//	{
//		//synchronize the wheels with the (interpolated) chassis worldtransform
//		m_vehicle->updateWheelTransform(i, true);
//		btTransform transform;
//		transform = m_vehicle->getWheelInfo(i).m_worldTransform;
//
//		p_phyObject->GetMotionState()->getWorldTransform(childTransform);
//	}
//	//gVehicleSteering = 1.f;
//
//	//btVector3 separation = Separation();
//
//	//gVehicleSteering = originalSteer;
//	//if (separation.getX() < 0.f)
//	//	gVehicleSteering = -originalSteer;
//	//std::cout << "SPEED: " << m_vehicle->getCurrentSpeedKmHour() << "\n";
//	{
//		int wheelIndex;
//		{
//			wheelIndex = 2;
//			m_vehicle->applyEngineForce(m_gEngineForce, wheelIndex);
//			m_vehicle->setBrake(m_gBreakingForce, wheelIndex);
//
//			wheelIndex = 3;
//			m_vehicle->applyEngineForce(m_gEngineForce, wheelIndex);
//			m_vehicle->setBrake(m_gBreakingForce, wheelIndex);
//
//		}
//
//		//wheelIndex = 2;
//		//for(int i = 0; i < 4; i++)
//
//		//wheelIndex = 3;
//		//m_vehicle->setBrake(gBreakingForce, wheelIndex);
//
//		wheelIndex = 0;
//		m_vehicle->setSteeringValue(m_gVehicleSteering, wheelIndex);
//		wheelIndex = 1;
//		m_vehicle->setSteeringValue(m_gVehicleSteering, wheelIndex);
//		//std::cout << "BRAKE: " << gBreakingForce << "\n";
//
//	}
//
//
//	//btTransform childTransform;
//	for (int i = 0; i < static_cast<int>(m_model->GetMeshes().size()); i++)
//	{
//		p_phyObject->GetMotionState()->getWorldTransform(childTransform);//m_compoundShape->getChildTransform(i);
//		m_model->GetMeshes()[i].SetTransform(childTransform);
//	}
//
//	//std::cout << "TANK LOCATION: " << childTransform.getOrigin().getX() << " " << childTransform.getOrigin().getY() << " " << childTransform.getOrigin().getZ() << "\n";
//
//	CalculateFront();
//	Move(deltaTime);
//}
//void Tank::FindSteeringDirection()
//{
//	CalculateFront();
//	btVector3 frontView(m_front.x, m_front.y, m_front.z);
//
//	btVector3 position = GetPosition();
//	m_destinationDir = m_destination - position;
//
//	m_yRotationNeeded = frontView.angle(m_destinationDir);// +GetTransform().getRotation().getY();
//
//	float cosvalue = frontView.dot(m_destinationDir);
//	float sinvalue = sqrt(1.f - pow(cosvalue, 2.f));
//
//	//yRotationNeeded = atan2(-destinationDir.getY(), destinationDir.getX());
//
//	m_yRotationNeeded *= 180.f / PI;
//
//	sinvalue = (frontView.getX() * m_destinationDir.getZ() - frontView.getZ() * m_destinationDir.getX()) / (frontView.length() * m_destinationDir.length());
//
//	if (sinvalue < 0.f)
//	{
//		m_steerFound = false;
//
//		std::cout << "LEFT\n";
//		m_gVehicleSteering = 0.5f;
//	}
//	else
//	{
//		m_steerFound = false;
//
//		std::cout << "RIGHT\n";
//		m_yRotationNeeded = 360.f - m_yRotationNeeded;
//		m_gVehicleSteering = -0.5f;
//	}
//	m_originalSteer = m_gVehicleSteering;
//
//	std::cout << "ROTATION: " << m_yRotationNeeded << " SINVALUE: " << sinvalue << "\n";
//}
//
//void Tank::SetDestination(bool nhasDestination, btVector3 ndestination)
//{
//	btVector3 position = GetPosition();
//	if (position.distance(ndestination) < 5.f)
//		return;
//	m_steerFound = false;
//	m_oldSteerAngle = 100.f;
//	m_hasDestination = nhasDestination;
//	m_destination = ndestination;
//	FindSteeringDirection();
//	m_gEngineForce = 150.f;
//	m_gBreakingForce = 0.f;
//}
//
//bool Tank::CheckSteer()
//{
//	if (!m_hasDestination)
//		return false;
//
//	CalculateFront();
//	btVector3 frontView(m_front.x, m_front.y, m_front.z);
//
//	float angle = frontView.angle(m_destinationDir) * 180.f / PI;
//	if (angle < 5.f)
//	{
//		//foundSteerStart = true;
//		if (angle > m_oldSteerAngle || angle < 3.f)
//			m_gVehicleSteering = 0.f;
//		m_oldSteerAngle = angle;
//		return true;
//	}
//	else if (angle > 7.5f)
//		FindSteeringDirection();
//	return false;
//}