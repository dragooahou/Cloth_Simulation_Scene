//
// Created by etien on 21/10/2020.
//

#include <glm/ext/matrix_transform.hpp>

#include "Rendering/Transform.h"

Transform::Transform() {
    float position[3] = {0, 0, 0};
    float rotation[3] = {0, 0, 0};
    float scale[3] = {1, 1, 1};
    mymath::matrix_t::RecomposeMatrixFromComponents(position, rotation, scale, matrix.m16);
}

Transform::~Transform() {}

void Transform::moveForward(float n) {
    matrix.Translation(n, 0, 0);
}


void Transform::move(Vec3 direction, float n) {
    matrix.Translation(direction * n);
}

void Transform::moveRight(float n) {
    matrix.Translation(0, 0, n);
}

void Transform::moveUp(float n) {
    matrix.Translation(0, n, 0);
}

void Transform::rotateX(float rot) {
    rotate(Vec3::RIGHT, rot);
}

void Transform::rotateY(float rot) {
    rotate(Vec3::UP, rot);
}


void Transform::rotateZ(float rot) {
    rotate(Vec3::FORWARD, rot);
}

void Transform::rotate(Vec3 axe, float angle) {
    angle = Vec3::radians(angle);
    matrix.RotationAxis(axe, angle);
}


Vec3 Transform::getPosition() const {
    return matrix.v.position;
}

void Transform::setPosition(const Vec3 &position) {
    matrix.v.position.Set(position.x, position.y, position.z, 1);
}

mymath::matrix_t& Transform::getTransform() {
    return matrix;
}

Vec3 Transform::getRotation() const {
    auto mat = matrix;
    mat.OrthoNormalize();
    return Vec3(mymath::RAD2DEG * atan2f(mat.m[1][2], mat.m[2][2]),
                mymath::RAD2DEG * atan2f(-mat.m[0][2], sqrtf(mat.m[1][2] * mat.m[1][2] + mat.m[2][2] * mat.m[2][2])),
                mymath::RAD2DEG * atan2f(mat.m[0][1], mat.m[0][0]));
}

Vec3 Transform::getScale() const {
    return Vec3(matrix.v.right.Length(),
                matrix.v.up.Length(),
                matrix.v.dir.Length());
}

nlohmann::json Transform::toJson() {
    nlohmann::json json;
    json.push_back(matrix.m16);
    return json;
}

Transform Transform::fromJson(nlohmann::json json) {
    Transform transform;

    for(int i = 0; i < 16; i++)
        transform.matrix.m16[i] = json[0][i];

    return transform;
}
