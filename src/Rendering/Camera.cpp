//
// Created by etien on 20/10/2020.
//

#include <cmath>
#include <API/OpenGL/Shader_Opengl.h>
#include <API/API3D.h>
#include "Rendering/Camera.h"

float Camera::aspect = 800.f/600.f;

void Camera::updateMatrix() {

    distance += (distanceTarget - distance) * SPEED_ZOOM_ANIM * ImGui::GetIO().DeltaTime;
    center += (centerTarget - center) * SPEED_CENTER_ANIM * ImGui::GetIO().DeltaTime;

    Vec3 vecteurDirecteur;
    vecteurDirecteur.x = std::cos(teta * PI / 180.0) * std::cos(phy * PI / 180.0);
    vecteurDirecteur.y = std::sin(phy * PI / 180.0);
    vecteurDirecteur.z = std::sin(teta * PI / 180.0) * std::cos(phy * PI / 180.0);

    position = vecteurDirecteur * distance + center;

    front = position - vecteurDirecteur;

    projectionMat = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
    viewMat = glm::lookAt(  glm::vec3(position.x, position.y, position.z),
                          (glm::vec3)front,
                            glm::vec3(0.0, 1.0, 0.0));

    API3D::instance->useBaseShader();
}


void Camera::setPosition(Vec3 position)
{
    distance = center.dist(position);
    distanceTarget = distance;
    teta = atan2(position.x, position.z) * 180.0 / PI;
    phy = atan2(position.y, sqrt(position.x * position.x + position.z * position.z)) * 180.0 / PI;
}

void Camera::rotateHorizontaly(float angle) {
    teta += angle;
}

void Camera::rotateVertically(float angle) {
    phy += angle;
    // limites
    if (phy > 90 - 1) phy = 90 - 1;
    if (phy < -90 + 1) phy = -90 + 1;
}

void Camera::setDistance(float distance)
{
    this->distance = distance;
    this->distanceTarget = distance;
}
float Camera::getDistance()
{
    return distance;
}

void Camera::zoom(float factor)
{
    distanceTarget = distance * factor;
}

void Camera::translateHorizontaly(double delta, double translateSpeedFactor)
{
    Vec3 translateVector(
        std::cos((teta + 90) * PI / 180.0) * delta * translateSpeedFactor *  distance,
        0,
        std::sin((teta + 90) * PI / 180.0) * delta * translateSpeedFactor *  distance
    );
    center += translateVector;
    centerTarget = center;
}

void Camera::translateVertically(double delta, double translateSpeedFactor)
{
    Vec3 translateVector(
        std::cos(teta * PI / 180.0) * std::cos((phy + 90) * PI / 180.0) * delta * translateSpeedFactor * distance,
        std::sin((phy + 90) * PI / 180.0) * delta * translateSpeedFactor * distance,
        std::sin(teta * PI / 180.0) * std::cos((phy + 90) * PI / 180.0) * delta * translateSpeedFactor * distance
    );
    center += translateVector;
    centerTarget = center;
}

void Camera::advance(double delta, double translateSpeedFactor)
{
    Vec3 translateVector(
        std::cos(teta * PI / 180.0) * std::cos(phy * PI / 180.0) * delta * translateSpeedFactor,
        std::sin(phy * PI / 180.0) * delta * translateSpeedFactor,
        std::sin(teta * PI / 180.0) * std::cos(phy * PI / 180.0) * delta * translateSpeedFactor
    );
    center += translateVector;
    centerTarget = center;
}

void Camera::setCenterPositionInstant(Vec3 pos)
{
    centerTarget = pos;
    center = pos;
}
void Camera::setCenterPosition(Vec3 pos)
{
    centerTarget = pos;
}

Vec3 Camera::getPosition() {
    return position;
}

float Camera::yaw() {
    return teta;
}
