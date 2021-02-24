//
// Created by etien on 21/10/2020.
//

#ifndef LSYSTEM_TRANSFORM_H
#define LSYSTEM_TRANSFORM_H


#include <glm/glm.hpp>
#include <future>
#include <nlohmann/json.hpp>
#include "Vec3.h"

class Transform {


public:

    Transform();
    ~Transform();

    void moveForward(float n); // z
    void moveRight(float n);   // x
    void moveUp(float n);      // y

    void move(Vec3 direction, float n); // absolute

    // Les paramètres en degrés
    void rotateX(float rot);
    void rotateY(float rot);
    void rotateZ(float rot);

    void rotate(Vec3 axe, float angle); // absolute

    void setPosition(const Vec3 &position);

    // Getters
    Vec3 getPosition() const;
    Vec3 getRotation() const;
    Vec3 getScale() const;


    mymath::matrix_t& getTransform();

    nlohmann::json toJson();
    static Transform fromJson(nlohmann::json json);

    mymath::matrix_t matrix;
};


#endif //LSYSTEM_TRANSFORM_H
