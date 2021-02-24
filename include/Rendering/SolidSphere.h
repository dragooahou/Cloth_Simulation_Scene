//
// Created by etien on 27/10/2020.
//

#ifndef PROJET_LSYSTEM_SOLIDSPHERE_H
#define PROJET_LSYSTEM_SOLIDSPHERE_H

#include "Mesh.h"

class SolidSphere : public Mesh {

public:
    SolidSphere(float radius, unsigned int rings, unsigned int sectors, Vec3 color);

};


#endif //PROJET_LSYSTEM_SOLIDSPHERE_H
