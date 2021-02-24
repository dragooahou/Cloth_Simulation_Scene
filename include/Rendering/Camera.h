//
// Created by etien on 20/10/2020.
//

#ifndef LSYSTEM_CAMERA_H
#define LSYSTEM_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Vec3.h"
#include "Transform.h"
#include "GameObject.h"
#include "Light.h"

class Camera{


public:

    float fov = 60, zNear = 0.001f, zFar = 100.0f;
    static float aspect;

    float teta = 0;
    float phy = 0;
    Vec3 center;
    Vec3 centerTarget;
    float distance = 0;
    float distanceTarget = 10;

    const double PI = 3.14159265359;

    Vec3 position;

    bool useLighting = true;

    Vec3 front;

public:

    void updateMatrix();

    void setPosition(Vec3 pos);
    void setCenterPositionInstant(Vec3 pos);
    void setCenterPosition(Vec3 pos);

    void rotateHorizontaly(float angle);
    void rotateVertically(float angle);

    void setDistance(float distance);
    float getDistance();
    void zoom(float factor);

    void translateHorizontaly(double delta, double translateSpeedFactor);
    void translateVertically(double delta, double translateSpeedFactor);
    void advance(double delta, double translateSpeedFactor);

    const float SPEED_ZOOM_ANIM = 10;
    const float SPEED_CENTER_ANIM = 5;

    // Les différentes matrices
    glm::mat4 projectionMat, viewMat;

    Vec3 getPosition();

    float yaw();

};


#endif //LSYSTEM_CAMERA_H
