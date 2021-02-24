//
// Created by etien on 03/02/2021.
//

#include <iostream>
#include "Simulation/Constraint.h"

void Constraint::satisfyConstraint() {
    Vec3 p1_to_p2 = (*p2)->getPosition() - (*p1)->getPosition(); // vector from p1 to p2
    float current_distance = p1_to_p2.mag(); // current distance between p1 and p2
    Vec3 correctionVector = p1_to_p2*(1 - rest_distance/current_distance); // The offset vector that could moves p1 into a distance of rest_distance to p2
    Vec3 correctionVectorHalf = correctionVector*0.5; // Lets make it half that length, so that we can move BOTH p1 and p2.
    (*p1)->offsetPos(correctionVectorHalf); // correctionVectorHalf is pointing from p1 to p2, so the length should move p1 half the length needed to satisfy the constraint.
    (*p2)->offsetPos(-correctionVectorHalf); // we must move p2 the negative direction of correctionVectorHalf since it points from p2 to p1, and not p1 to p2.
}

Constraint::Constraint(std::shared_ptr<Particle> *p1, std::shared_ptr<Particle> *p2) :  p1(p1), p2(p2) {
    Vec3 vec = (*p1)->getPosition() - (*p2)->getPosition();
    rest_distance = vec.mag();
}
