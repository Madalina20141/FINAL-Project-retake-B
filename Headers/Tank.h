#pragma once
//#include "Headers/ObjectModel.h"
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wfloat-conversion"
//#include <bullet/btBulletDynamicsCommon.h>
//#pragma GCC diagnostic pop
//
//class ModelManager;
//class btCompoundShape;
//class CubeModel;
//
//class Tank : public ObjectModel
//{
//public:
//    Tank(glm::vec3 position, glm::vec3 rotation, float nmovementSpeed, ModelManager* nmodel, int nteam);
//    void Update(float deltaTime);
//    void FindSteeringDirection();
//    void SetDestination(bool nhasDestination, btVector3 ndestination);
//    bool CheckSteer();
//private:
//    bool m_isSelected = false;
//    float m_currentCoolodwn = 0.f;
//    ModelManager* m_model;
//    btRaycastVehicle::btVehicleTuning    m_tuning;
//    btVehicleRaycaster* m_vehicleRayCaster;
//    btRaycastVehicle* m_vehicle;
//
//    btCompoundShape* m_compoundShape;
//    glm::vec3 m_front;
//
//    float    m_gEngineForce = 0.f;
//    float    m_gBreakingForce = 1000.f;
//
//    float    m_maxEngineForce = 1000.f;//this should be engine/velocity dependent
//    float    m_maxBreakingForce = 1000.f;
//
//    float    m_gVehicleSteering = 0.f;
//    float    m_steeringIncrement = 0.04f;
//    float    m_steeringClamp = 0.3f;
//    float    m_wheelRadius = 0.5f;
//    float    m_wheelWidth = 0.4f;
//    float    m_wheelFriction = 1000;//BT_LARGE_FLOAT;
//    float    m_suspensionStiffness = 20.f;
//    float    m_suspensionDamping = 2.3f;
//    float    m_suspensionCompression = 2.4f;
//    float    m_rollInfluence = 0.1f;//1.0f;
//
//    float m_oldSteerAngle = 100.f;
//    btScalar m_suspensionRestLength = 0.6f;
//    btVector3 m_wheelDirectionCS0 = btVector3(0, -1, 0);
//    btVector3 m_wheelAxleCS = btVector3(-1, 0, 0);
//
//    std::vector <CubeModel*> wheelsCube;
//    btVector3 m_destination;
//    btVector3 m_destinationDir;
//    bool m_hasDestination = false;
//    bool m_steerFound = false;
//    float m_yRotationNeeded = 0.f;
//
//    Tank* target = nullptr;
//    float m_shootCooldown = 5.f;
//    float m_originalSteer;
//    float m_recalculateSteerTimer = 5.f;
//    float m_currentSteerTimer = 0.f;
//
//    glm::vec4 m_hueChange = glm::vec4(1.0);
//
//};