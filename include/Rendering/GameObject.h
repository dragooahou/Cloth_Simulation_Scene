//
// Created by etien on 05/11/2020.
//

#ifndef PROJET_LSYSTEM_GAMEOBJECT_H
#define PROJET_LSYSTEM_GAMEOBJECT_H

#include <Component.h>
#include "Mesh.h"
#include "Transform.h"
#include "Material.h"
#include "RenderingChain.h"
#include "Model.h"
#include "nlohmann/json.hpp"

class GameObject {

    std::vector<std::unique_ptr<Component>> components;

public:
    GameObject();

    Transform transform;
    std::vector<Model> models;

    virtual void update(float dt);
    virtual void render(RenderingChain* renderChain);

    std::string name = "GameObject";

    void addComponent(Component* comp);
    void removeComponent(Component* comp);

    const std::vector<std::unique_ptr<Component>> &getComponents() const;

    virtual nlohmann::json toJson();
    static GameObject* fromJson(nlohmann::json json);
};


#endif //PROJET_LSYSTEM_GAMEOBJECT_H
