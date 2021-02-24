//
// Created by etien on 18/09/2020.
//

#include "Rendering/Vec3.h"

#include <cmath>

float lerputil(float start, float stop, float  amt) {
    return start + (stop-start) * amt;
}

const Vec3 Vec3::UP = Vec3(0, 1, 0);
const Vec3 Vec3::DOWN = Vec3(0, -1, 0);
const Vec3 Vec3::FORWARD = Vec3(0, 0, 1);
const Vec3 Vec3::BACKWARD = Vec3(0, 0, -1);
const Vec3 Vec3::RIGHT = Vec3(1, 0, 0);
const Vec3 Vec3::LEFT = Vec3(-1, 0, 0);
const Vec3 Vec3::ZERO = Vec3(0, 0, 0);

Vec3::Vec3() : x(0), y(0), z(0) {}

Vec3::Vec3(float x, float y) : x(x), y(y), z(0) {}

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

Vec3 *Vec3::set(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
    return this;
}

Vec3 *Vec3::set(Vec3 v) {
    x = v.x;
    y = v.y;
    z = v.z;
    return this;
}

Vec3 Vec3::random2D() {
    return fromAngle2D(std::rand() * M_PI * 2);
}

Vec3 Vec3::fromAngle2D(float angle) {
    return Vec3(std::cos(angle), std::sin(angle));
}

Vec3 *Vec3::setRandom2D() {
    return setFromAngle2D(std::rand() * M_PI * 2);
}

Vec3 *Vec3::setFromAngle2D(float angle) {
    x = std::cos(angle);
    y = std::sin(angle);
    return this;
}

Vec3 Vec3::copy() const {
    return Vec3(x, y, z);
}

float Vec3::mag() const {
    return std::sqrt(x * x + y * y + z * z);
}

float Vec3::magSq() const {
    return (x * x + y * y + z * z);
}

Vec3 *Vec3::add(Vec3 v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return this;
}

Vec3 *Vec3::add(float x, float y, float z) {
    this->x += x;
    this->y += y;
    this->z += z;
    return this;
}

Vec3 Vec3::operator+(Vec3 other) const {
    return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 *Vec3::sub(Vec3 v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return this;
}

Vec3 *Vec3::sub(float x, float y, float z) {
    this->x -= x;
    this->y -= y;
    this->z -= z;
    return this;
}

Vec3 Vec3::operator-(Vec3 other) const {
    return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 *Vec3::mult(float n) {
    x *= n;
    y *= n;
    z *= n;
    return this;
}

Vec3 Vec3::operator*(float n) const {
    return Vec3(x * n, y * n, z * n);
}

Vec3 *Vec3::div(float n) {
    x /= n;
    y /= n;
    z /= n;
    return this;
}

Vec3 Vec3::operator/(float n) const {
    return Vec3(x / n, y / n, z / n);
}

float Vec3::dist(Vec3 v) const {
    float dx = x - v.x;
    float dy = y - v.y;
    float dz = z - v.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

float Vec3::distSq(Vec3 v) const {
    float dx = x - v.x;
    float dy = y - v.y;
    float dz = z - v.z;
    return dx * dx + dy * dy + dz * dz;
}

float Vec3::dot(Vec3 v) const {
    return x * v.x + y * v.y + z*v.z;
}

float Vec3::dot(float x, float y, float z) const {
    return this->x * x + this->y * y + this->z * z;
}

Vec3 *Vec3::normalize() {
    float m = mag();
    if (m != 0 && m != 1) {
        div(m);
    }
    return this;
}

Vec3 *Vec3::limit(float max) {
    if (magSq() > max * max) {
        normalize();
        mult(max);
    }
    return this;
}

Vec3 *Vec3::setMag(float len) {
    normalize();
    mult(len);
    return this;
}

float Vec3::heading2D() const {
    return std::atan2(y, x);
}

Vec3 *Vec3::rotate2D(float theta) {
    return rotateZ(theta);
}

Vec3 *Vec3::rotateZ(float theta) {
    float temp = x;
    x = x * std::cos(theta) - y * std::sin(theta);
    y = temp * std::sin(theta) + y * std::cos(theta);
    return this;
}

Vec3 *Vec3::rotateY(float theta) {
    float temp = x;
    x = x * std::cos(theta) - z * std::sin(theta);
    z = temp * std::sin(theta) + z * std::cos(theta);
    return this;
}

Vec3 *Vec3::rotateX(float theta) {
    float temp = y;
    y = y * std::cos(theta) - z * std::sin(theta);
    z = temp * std::sin(theta) + z * std::cos(theta);
    return this;
}

Vec3 *Vec3::rotate(const Vec3& axis, float theta) {
    const float cos_theta = std::cos(theta);
    const float dot = this->dot(axis);
    Vec3 cross = Vec3::cross(*this, axis);
    *this *= cos_theta;
    *this += axis * dot * (1.0f - cos_theta);
    *this -= cross * std::sin(theta);
    return this;
}

Vec3 *Vec3::lerp(Vec3 v, float amt) {
    x = lerputil(x, v.x, amt);
    y = lerputil(y, v.y, amt);
    z = lerputil(z, v.z, amt);
    return this;
}

Vec3 Vec3::lerp(Vec3 v1, Vec3 v2, float amt) {
    Vec3 v = v1.copy();
    v.lerp(v2, amt);
    return v;
}

Vec3 *Vec3::lerp(float x, float y, float z, float amt) {
    this->x = lerputil(this->x, x, amt);
    this->y = lerputil(this->y, y, amt);
    this->z = lerputil(this->z, z, amt);
    return this;
}

float Vec3::angleBetween(Vec3 v1, Vec3 v2) {
    if (v1.x == 0 && v1.y == 0 && v1.z == 0 ) return 0.0f;
    if (v2.x == 0 && v2.y == 0 && v2.z == 0 ) return 0.0f;

    double dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    double v1mag = std::sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
    double v2mag = std::sqrt(v2.x * v2.x + v2.y * v2.y + v2.z * v2.z);
    double amt = dot / (v1mag * v2mag);
    if (amt <= -1)
        return M_PI;
    else if (amt >= 1)
        return 0;

    return (float) acos(amt);
}

bool Vec3::operator==(Vec3 p) const {
    return x == p.x && y == p.y && z == p.z;
}

Vec3 Vec3::operator-() const {
    return Vec3(-x, -y, -z);
}

Vec3& Vec3::operator=(Vec3 other) {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

Vec3 operator*(float f, Vec3 v) {
    return Vec3(v.x*f, v.y*f, v.z*f);
}

Vec3& Vec3::operator-=(Vec3 other){
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vec3& Vec3::operator+=(Vec3 other){
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vec3& Vec3::operator*=(float other){
    x *= other;
    y *= other;
    z *= other;
    return *this;
}

Vec3 Vec3::cross(Vec3 v1, Vec3 v2) {
    float crossX = v1.y * v2.z - v2.y * v1.z;
    float crossY = v1.z * v2.x - v2.z * v1.x;
    float crossZ = v1.x * v2.y - v2.x * v1.y;
    return Vec3(crossX, crossY, crossZ);
}

float& Vec3::operator[](int i) {
    switch (i) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            throw std::out_of_range("Vec3 index out of bounds : " + std::to_string(i));
    }
}

Vec3::operator glm::vec3() const {
    return glm::vec3(x, y, z);
}

Vec3::operator mymath::vec_t() const {
    return mymath::vec_t{x, y, z};
}

Vec3 Vec3::normalized() {
    float m = mag();
    if (m != 0 && m != 1)
        return *this / m;
    return Vec3(*this);
}

Vec3::Vec3(mymath::vec_t vec) : x(vec.x), y(vec.y), z(vec.z){}

Vec3 &Vec3::operator=(glm::vec3 other) {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

Vec3::operator glm::vec4() const {
    return glm::vec4(x, y, z, 0);
}

std::ostream& operator<<(std::ostream& ostream, Vec3 v) {
    ostream << "Vec3{ "<<v.x<<", "<<v.y<<", "<<v.z<<" }";
    return ostream;
}
