#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h" 
#include <cmath>

class MousePicker
{
public:
    MousePicker(Camera* cam, glm::mat4 projection);
    ~MousePicker();

    bool intersectionInRange(float start, float end, glm::vec3 ray);
    bool isUnderground(glm::vec3 testPoint);

    glm::vec3 getCurrentRay() {
        return currentRay;
    };

    glm::vec3 getCurrentTerrainPoint() {
        return currentTerrainPoint;
    }

    void Update();
private:
    const int RECURSION_COUNT = 200;
    const float RAY_RANGE = 600;
    int windowWidth = 1080;
    int windowHeight = 1920;

    glm::vec3 currentRay;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    Camera* camera;

    glm::vec3 currentTerrainPoint;

    glm::vec3 binarySearch(int count, float start, float end, glm::vec3 ray);

    glm::vec3 calculateMouseRay()
    {
        double mouseX, mouseY;
        // Get mouse position here
        glm::vec2 normalizedCoords = getNormalisedDeviceCoordinates(mouseX, mouseY);
        glm::vec4 clipCoords = glm::vec4(normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f);
        glm::vec4 eyeCoords = toEyeCoords(clipCoords);
        glm::vec3 worldRay = toWorldCoords(eyeCoords);
        return worldRay;
    }

    glm::vec3 toWorldCoords(glm::vec4 eyeCoords)
    {
        glm::mat4 invertedView = glm::inverse(viewMatrix);
        glm::vec4 rayWorld = invertedView * eyeCoords;
        glm::vec3 mouseRay = glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z);
        mouseRay = glm::normalize(mouseRay);
        return mouseRay;
    }

    glm::vec4 toEyeCoords(glm::vec4 clipCoords)
    {
        glm::mat4 invertedProjection = glm::inverse(projectionMatrix);
        glm::vec4 eyeCoords = invertedProjection * clipCoords;
        return glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
    }

    glm::vec2 getNormalisedDeviceCoordinates(double mouseX, double mouseY)
    {
        float x = (2.0f * mouseX) / windowWidth - 1.0f;
        float y = (2.0f * mouseY) / windowHeight - 1.0f;
        return glm::vec2(x, y);
    }

    glm::vec3 getPointOnRay(glm::vec3 ray, float distance)
    {
        glm::vec3 camPos = camera->Position;
        glm::vec3 start = glm::vec3(camPos.x, camPos.y, camPos.z);
        glm::vec3 scaledRay = glm::vec3(ray.x * distance, ray.y * distance, ray.z * distance);
        return start + scaledRay;
    }
};