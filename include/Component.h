//
// Created by etien on 04/02/2021.
//

#ifndef CLOTHSIM_COMPONENT_H
#define CLOTHSIM_COMPONENT_H

#include <string>
#include <iostream>
#include <utility>
#include <map>
#include "Rendering/RenderingChain.h"
#include "nlohmann/json.hpp"

class GameObject;


struct Component {

    typedef std::function<Component*()> InstantiateComponentFunction;
    static std::map<std::string, InstantiateComponentFunction> instanciers;

protected:


    Component(const std::string& name) : name(name) {}

public:
    std::string name;
    GameObject* gameObject = nullptr;

    virtual ~Component();

    static void init();

    virtual void update() {}
    virtual void render(RenderingChain* renderingChain) {}
    virtual void renderGUI() {}
    virtual nlohmann::json toJson();
    virtual void parseJson(nlohmann::json json);

    static const std::map<std::string, InstantiateComponentFunction> &getInstanciers();
};


#endif //CLOTHSIM_COMPONENT_H
