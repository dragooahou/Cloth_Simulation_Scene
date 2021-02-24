//
// Created by etien on 03/02/2021.
//

#ifndef CLOTHSIM_PARTICLE_H
#define CLOTHSIM_PARTICLE_H

#include <functional>

#include <Rendering/Vec3.h>

class Particle {

public:
    Vec3 position;
    Vec3 old_position;
    Vec3 acceleration;
    bool movable;
    float mass;
    Vec3 accumulated_normal;

    std::function<const Vec3(void)> getAttachedPosition = []() {
        return Vec3();
    };

    Particle(Vec3 pos) : position(pos), old_position(pos),acceleration(Vec3(0,0,0)), mass(1), movable(true), accumulated_normal(Vec3{0, 0, 0}) {}
    Particle(){}


    void addForce(Vec3 f);

    void timeStep();

    void offsetPos(Vec3 offset);

    void makeMovable(bool movable);

    void addToNormal(Vec3 normal);

    void resetNormal();

    Vec3 getPosition();

};


#endif //CLOTHSIM_PARTICLE_H
