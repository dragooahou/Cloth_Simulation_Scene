//
// Created by etien on 04/02/2021.
//

#include <Simulation/Cloth.h>
#include <Rendering/Light.h>
#include "Component.h"

const std::map<std::string, Component::InstantiateComponentFunction> &Component::getInstanciers() {
    return instanciers;
}

Component::~Component() {

}

void Component::init() {

    instanciers["Cloth"] = [](){
        return new Cloth(0.05,1,4,45);
    };

    instanciers["Light"] = [](){
        return new Light();
    };

    instanciers["Wind"] = [](){
        return new Cloth::Wind();
    };

}

nlohmann::json Component::toJson() {
    nlohmann::json json;
    json["name"] = name;
    return json;
}

void Component::parseJson(nlohmann::json json) {}
