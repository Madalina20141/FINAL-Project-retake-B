#include "../Headers/MousePicker.h"

MousePicker::MousePicker(Camera* cam, glm::mat4 projection) {
    camera = cam;
    projectionMatrix = projection;
    viewMatrix = glm::lookAt(camera->Position, camera->Position + camera->Front, camera->Up);
}

MousePicker::~MousePicker() {
}

void MousePicker::Update() {
    viewMatrix = glm::lookAt(camera->Position, camera->Position + camera->Front, camera->Up);
    currentRay = calculateMouseRay();
    if (intersectionInRange(0, RAY_RANGE, currentRay)) {
        currentTerrainPoint = binarySearch(0, 0, RAY_RANGE, currentRay);
    }
    else {
        currentTerrainPoint = glm::vec3(0, 0, 0);
    }
}

bool MousePicker::intersectionInRange(float start, float end, glm::vec3 ray) {
    glm::vec3 startPoint = getPointOnRay(ray, start);
    glm::vec3 endPoint = getPointOnRay(ray, end);
    if (!isUnderground(startPoint) && isUnderground(endPoint)) {
        return true;
    }
    else {
        return false;
    }
}

bool MousePicker::isUnderground(glm::vec3 testPoint) {
    // Check if testPoint is below the terrain height
    return false;
}

glm::vec3 MousePicker::binarySearch(int count, float start, float end, glm::vec3 ray) {
    float half = start + ((end - start) / 2.0f);
    if (count >= RECURSION_COUNT) {
        glm::vec3 endPoint = getPointOnRay(ray, half);
        return endPoint;
    }
    if (intersectionInRange(start, half, ray)) {
        return binarySearch(count + 1, start, half, ray);
    }
    else {
        return binarySearch(count + 1, half, end, ray);
    }
}

