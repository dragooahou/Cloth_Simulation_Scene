//
// Created by etien on 03/02/2021.
//

#include "Simulation/Particle.h"

float DAMPING = 0.01; // how much to damp the cloth simulation each frame
float TIME_STEPSIZE2 = 0.25; // how large time step each particle takes each frame

void Particle::addForce(Vec3 f) {
    acceleration += f/mass;
}

void Particle::timeStep() {
    if(movable) {
        Vec3 temp = position;
        position = position + (position - old_position) * (1-DAMPING) + acceleration*TIME_STEPSIZE2;
        old_position = temp;
        acceleration = Vec3(0,0,0); // acceleration is reset since it HAS been translated into a change in position (and implicitely into velocity)
    }
}

void Particle::makeMovable(bool movable) {
    this->movable = movable;
}

void Particle::offsetPos(Vec3 offset) {
    if(movable)
        position += offset;
}

void Particle::addToNormal(Vec3 normal) {
    accumulated_normal += normal.normalized();
}

void Particle::resetNormal() {
    accumulated_normal = Vec3(0,0,0);
}

Vec3 Particle::getPosition() {
    if(getAttachedPosition)
        return getAttachedPosition() + position;
    else
        return position;
}
