//
// Created by etien on 05/02/2021.
//

#ifndef CLOTHSIM_MATRIX_H
#define CLOTHSIM_MATRIX_H

#include <cstring>
#include <cfloat>
#include <glm/glm.hpp>

namespace mymath {

static const float ZPI = 3.14159265358979323846f;
static const float RAD2DEG = (180.f / ZPI);
static const float DEG2RAD = (ZPI / 180.f);


template <typename T> T Clamp(T x, T y, T z) { return ((x < y) ? y : ((x > z) ? z : x)); }
template <typename T> T max(T x, T y) { return (x > y) ? x : y; }
template <typename T> T min(T x, T y) { return (x < y) ? x : y; }
template <typename T> bool IsWithin(T x, T y, T z) { return (x >= y) && (x <= z); }

struct matrix_t;
struct vec_t
{
public:
    float x, y, z, w;

    void Lerp(const vec_t& v, float t);

    void Set(float v);
    void Set(float _x, float _y, float _z = 0.f, float _w = 0.f);

    vec_t& operator -= (const vec_t& v);
    vec_t& operator += (const vec_t& v);
    vec_t& operator *= (const vec_t& v);
    vec_t& operator *= (float v);

    vec_t operator * (float f) const;
    vec_t operator - () const;
    vec_t operator - (const vec_t& v) const;
    vec_t operator + (const vec_t& v) const;
    vec_t operator * (const vec_t& v) const;

    const vec_t& operator + () const;
    float Length() const;;
    float LengthSq() const;;
    vec_t Normalize();
    vec_t Normalize(const vec_t& v);
    vec_t Abs() const;

    void Cross(const vec_t& v);

    void Cross(const vec_t& v1, const vec_t& v2);

    float Dot(const vec_t& v) const;

    float Dot3(const vec_t& v) const;

    void Transform(const matrix_t& matrix);
    void Transform(const vec_t& s, const matrix_t& matrix);

    void TransformVector(const matrix_t& matrix);
    void TransformPoint(const matrix_t& matrix);
    void TransformVector(const vec_t& v, const matrix_t& matrix);
    void TransformPoint(const vec_t& v, const matrix_t& matrix);

    float& operator [] (size_t index);
    const float& operator [] (size_t index) const;
    bool operator!=(const vec_t& other) const;

};

vec_t Normalized(const vec_t& v);
vec_t Cross(const vec_t& v1, const vec_t& v2);

float Dot(const vec_t& v1, const vec_t& v2);

vec_t BuildPlan(const vec_t& p_point1, const vec_t& p_normal);

    void LookAt(const float *eye, const float *at, const float *up, float *m16);

struct matrix_t
{
public:

    union
    {
        float m[4][4];
        float m16[16];
        struct
        {
            vec_t right, up, dir, position;
        } v;
        vec_t component[4];
    };

    matrix_t(const matrix_t& other);
    matrix_t();

    operator float* ();
    operator const float* () const;
    void Translation(float _x, float _y, float _z);

    void Translation(const vec_t& vt);

    void Scale(float _x, float _y, float _z);
    void Scale(const vec_t& s);

    matrix_t& operator *= (const matrix_t& mat);
    matrix_t operator * (const matrix_t& mat) const;

    void Multiply(const matrix_t& matrix);

    void Multiply(const matrix_t& m1, const matrix_t& m2);

    float GetDeterminant() const;

    float Inverse(const matrix_t& srcMatrix, bool affine = false);
    void SetToIdentity();
    void Transpose();

    void RotationAxis(const vec_t& axis, float angle);

    void OrthoNormalize();

    static void RecomposeMatrixFromComponents(const float* translation, const float* rotation, const float* scale, float* matrix);

    static void DecomposeMatrixToComponents(const float* matrix, float* translation, float* rotation, float* scale);

};



}

#endif //CLOTHSIM_MATRIX_H
