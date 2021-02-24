//
// Created by etien on 18/09/2020.
//

#ifndef PHYSICSENGINE2D_Vec3_H
#define PHYSICSENGINE2D_Vec3_H


#include <cstdlib>
#include <cmath>
#include <ostream>
#include <glm/vec3.hpp>
#include "Matrix.h"

class Vec3 {

public:

    float x, y, z;

    Vec3();
    Vec3(float x, float y);
    Vec3(float x, float y, float z);
    Vec3(mymath::vec_t vec);

    Vec3* set(Vec3 v);

    static Vec3 random2D();
    static Vec3 fromAngle2D(float angle);
    Vec3* setRandom2D();
    Vec3* setFromAngle2D(float angle);

    Vec3 copy() const;

    // aka length
    float mag() const;
    float magSq() const;

    Vec3* add(Vec3 v);
    Vec3 operator+(Vec3 other) const;

    Vec3* sub(Vec3 v);
    Vec3 operator-(Vec3 other) const;

    Vec3* mult(float n);
    Vec3 operator*(float n) const;

    Vec3* div(float n);
    Vec3 operator/(float n) const;

    float dist(Vec3 v) const;

    float dot(Vec3 v) const;

    Vec3* normalize();
    Vec3 normalized();

    Vec3* limit(float max);

    Vec3* setMag(float len);

    // get rotation angle
    float heading2D() const;

    Vec3* rotate2D(float theta);

    Vec3* lerp(Vec3 v, float amt);
    static Vec3 lerp(Vec3 v1, Vec3 v2, float amt);

    static float angleBetween(Vec3 v1, Vec3 v2);

    bool operator==(Vec3 p) const;

    Vec3 operator-() const;

    Vec3& operator=(Vec3 other);
    Vec3& operator=(glm::vec3 other);

    Vec3& operator-=(Vec3 other);
    Vec3& operator+=(Vec3 other);
    Vec3& operator*=(float other);

    Vec3 *add(float x, float y, float z);

    Vec3 *set(float x, float y, float z);

    Vec3 *sub(float x, float y, float);

    float dot(float x, float y, float z) const;

    Vec3 *lerp(float x, float y, float z, float amt);

    static Vec3 cross(Vec3 v1, Vec3 v2);

    constexpr static float DEG_TO_RAD = M_PI/180.0f;
    constexpr static float RAD_TO_DEG = 180.0f/M_PI;

    float static radians(float deg) {
        return deg*DEG_TO_RAD;
    }

    float static degrees(float rad) {
        return rad*RAD_TO_DEG;
    }

    float distSq(Vec3 v) const;

    Vec3 *rotateZ(float theta);

    Vec3 *rotateX(float theta);

    Vec3 *rotateY(float theta);

    static const Vec3 UP;
    static const Vec3 DOWN;
    static const Vec3 FORWARD;
    static const Vec3 BACKWARD;
    static const Vec3 RIGHT;
    static const Vec3 LEFT;
    static const Vec3 ZERO;

    Vec3 *rotate(const Vec3 &axis, float theta);

    float& operator[](int i);

    operator glm::vec3() const;
    operator glm::vec4() const;
    operator mymath::vec_t() const;

};

Vec3 operator*(float f, Vec3 v);
std::ostream& operator<<(std::ostream& ostream, Vec3 v);

#endif //PHYSICSENGINE2D_Vec3_H
