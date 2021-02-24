//
// Created by etien on 03/02/2021.
//

#ifndef CLOTHSIM_CONSTRAINT_H
#define CLOTHSIM_CONSTRAINT_H

#include <memory>
#include "Particle.h"

class Constraint {

public:
    float rest_distance;
    std::shared_ptr<Particle>* p1;
    std::shared_ptr<Particle>* p2;

    glm::vec3 p1_pos;
    glm::vec3 p2_pos;

    Constraint(std::shared_ptr<Particle> *p1, std::shared_ptr<Particle> *p2);

    void satisfyConstraint();

    struct GLSLStruct {
        float rest_dist;
        glm::vec3 p1_pos;
        glm::vec3 p2_pos;
    };

};

#endif //CLOTHSIM_CONSTRAINT_H
