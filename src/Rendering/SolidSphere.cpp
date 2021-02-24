//
// Created by etien on 27/10/2020.
//

#include "Rendering/SolidSphere.h"

SolidSphere::SolidSphere(float radius, unsigned int rings, unsigned int sectors, Vec3 col) {
    color = col;
    float const R = 1.f/(float)(rings-1);
    float const S = 1.f/(float)(sectors-1);
    int r, s;

    vertices.resize(rings * sectors * 3);
    normales.resize(rings * sectors * 3);
    texcoords.resize(rings * sectors * 2);
    colors.resize(rings * sectors * 3);
    auto v = vertices.begin();
    auto n = normales.begin();
    auto t = texcoords.begin();
    auto c = colors.begin();
    for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
            float const y = sin( -M_PI_2 + M_PI * r * R );
            float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
            float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

            *t++ = s*S;
            *t++ = r*R;

            *v++ = x * radius;
            *v++ = y * radius;
            *v++ = z * radius;

            *n++ = x;
            *n++ = y;
            *n++ = z;

            *c++ = color.x;
            *c++ = color.y;
            *c++ = color.z;
        }

    indices.resize(rings * sectors * 6);
    auto i = indices.begin();
    for(r = 0; r < rings-1; r++)
        for(s = 0; s < sectors-1; s++) {
            *i++ = (r+1) * sectors + s;
            *i++ = (r+1) * sectors + (s+1);
            *i++ = r * sectors + (s+1);

            *i++ = (r+1) * sectors + s;
            *i++ = r * sectors + (s+1);
            *i++ = r * sectors + s;
        }

    updateBuffers();
}