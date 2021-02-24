//
// Created by etien on 05/11/2020.
//

#include <API/API3D.h>
#include "Rendering/GameObject.h"
#include "Rendering/Camera.h"

void GameObject::render(RenderingChain* renderChain) {
    
    for(auto& model : models)
        model.render(renderChain, &transform);

    for(auto& comp : components)
        comp->render(renderChain);
}

void GameObject::update(float dt) {
    for(auto& comp : components)
        comp->update();
}

void GameObject::addComponent(Component *comp) {
    comp->gameObject = this;
    components.emplace_back(comp);
}

const std::vector<std::unique_ptr<Component>> &GameObject::getComponents() const {
    return components;
}

void GameObject::removeComponent(Component *comp) {
    for(int i = 0; i < components.size(); i++)
        if(comp == components[i].get()) {
            components.erase(components.begin()+i);
            break;
        }
}

GameObject::GameObject() {}

nlohmann::json GameObject::toJson() {
    nlohmann::json json;

    json["name"] = name;
    json["transform"] = transform.toJson();

    for(auto& model : models)
        json["models"].push_back(model.toJson());

    for(auto& comp : components)
        json["components"].push_back(comp->toJson());

    return json;
}

GameObject* GameObject::fromJson(nlohmann::json json) {
    auto* gameObject = new GameObject();

    gameObject->name = json["name"];

    gameObject->transform = Transform::fromJson(json["transform"]);

    for(auto& model : json["models"])
        gameObject->models.push_back(Model::fromJson(model));

    for(auto& comp : json["components"]) {
        auto inst = Component::getInstanciers();

        auto* newcomp = inst[comp["name"]]();
        newcomp->parseJson(comp);
        gameObject->addComponent(newcomp);
    }

    return gameObject;
};
