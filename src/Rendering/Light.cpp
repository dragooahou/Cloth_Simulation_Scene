//
// Created by etien on 04/02/2021.
//

#include <API/API3D.h>
#include <API/OpenGL/Shader_Opengl.h>
#include <sstream>
#include "Rendering/Light.h"
#include "Rendering/Camera.h"
#include "Rendering/GameObject.h"

std::map<std::string, Component::InstantiateComponentFunction> Component::instanciers;
std::vector<Light*> Light::alllights;

glm::mat4 extractRotation(glm::mat4 r) {
    glm::mat4 m = r;
    m[3] = glm::vec4(0, 0, 0, 1);
    float s1 = length(m[0]);
    float s2 = length(m[1]);
    float s3 = length(m[2]);
    m[0] = m[0] / s1;
    m[1] = m[1] / s2;
    m[2] = m[2] / s3;
    return m;
}


void Light::render(RenderingChain* renderingChain) {

    renderingChain->addStep([this](Camera* camera) {
        Vec3 p = gameObject->transform.getPosition() + position;

        std::ostringstream prefixss;
        prefixss << "light[" << light_id << "].";
        auto prefix = prefixss.str();

        API3D::instance->uniform1iv(Shader_Opengl::baseShader(), (prefix+"type").c_str(), (int*)&type, 1);


        switch (type) {

            case Direction:
                API3D::instance->uniform3fv(Shader_Opengl::baseShader(), (prefix+"direction").c_str(), &direction[0], 1);
                API3D::instance->uniform3fv(Shader_Opengl::baseShader(), (prefix+"color").c_str(), &color[0], 1);
                break;

            case Point:
                API3D::instance->uniform3fv(Shader_Opengl::baseShader(), (prefix+"position").c_str(), &p[0], 1);

                API3D::instance->uniform1fv(Shader_Opengl::baseShader(), (prefix+"constant").c_str(), &constant, 1);
                API3D::instance->uniform1fv(Shader_Opengl::baseShader(), (prefix+"linear").c_str(), &linear, 1);
                API3D::instance->uniform1fv(Shader_Opengl::baseShader(), (prefix+"quadratic").c_str(), &quadratic, 1);

                API3D::instance->uniform3fv(Shader_Opengl::baseShader(), (prefix+"color").c_str(), &color[0], 1);
                break;

            case Spot:
                API3D::instance->uniform3fv(Shader_Opengl::baseShader(), (prefix+"position").c_str(), &p[0], 1);
                API3D::instance->uniform3fv(Shader_Opengl::baseShader(), (prefix+"direction").c_str(), &direction[0], 1);

                API3D::instance->uniform1fv(Shader_Opengl::baseShader(), (prefix+"cutOff").c_str(), &cutOff, 1);
                API3D::instance->uniform1fv(Shader_Opengl::baseShader(), (prefix+"outerCutOff").c_str(), &outerCutOff, 1);

                API3D::instance->uniform1fv(Shader_Opengl::baseShader(), (prefix+"constant").c_str(), &constant, 1);
                API3D::instance->uniform1fv(Shader_Opengl::baseShader(), (prefix+"linear").c_str(), &linear, 1);
                API3D::instance->uniform1fv(Shader_Opengl::baseShader(), (prefix+"quadratic").c_str(), &quadratic, 1);

                API3D::instance->uniform3fv(Shader_Opengl::baseShader(), (prefix+"color").c_str(), &color[0], 1);
                break;
        }


        API3D::instance->uniform3fv(Shader_Opengl::baseShader(), "viewPos", &camera->getPosition()[0], 1);
    });

}

Light::Light() : Component("Light"), light_id(alllights.size()){

    alllights.push_back(this);

    int size = alllights.size();
    API3D::instance->uniform1iv(Shader_Opengl::baseShader(), "nbLights", &size, 1);
}

void Light::renderGUI() {
    Component::renderGUI();

    ImGui::Text("%s", name.c_str());

    if(ImGui::BeginCombo("Type", typenames[type].c_str())) {
        for(int i = 0; i < typenames.size(); i++) {
            bool selected = type == i;
            ImGui::Selectable(typenames[i].c_str(), &selected);
            if(selected && type != i) type = static_cast<Type>(i);
        }
        ImGui::EndCombo();
    }

    switch (type) {

        case Direction:
            ImGui::InputFloat3("Direction", &direction[0]);
            ImGui::ColorEdit3("Color", &color[0]);
            break;


        case Point:
            ImGui::InputFloat3("Position", &position[0]);
            ImGui::ColorEdit3("Color", &color[0]);

            ImGui::InputFloat("Constant", &constant);
            ImGui::InputFloat("Linear", &linear, 0.01, 0.1, "%.6f");
            ImGui::InputFloat("Quadratic", &quadratic, 0.01, 0.1, "%.6f");

            break;

        case Spot:
            ImGui::InputFloat3("Position", &position[0]);
            ImGui::InputFloat3("Direction", &direction[0]);
            ImGui::ColorEdit3("Color", &color[0]);

            ImGui::InputFloat("Cut Off", &cutOff, 0.01, 0.1, "%.6f");
            ImGui::InputFloat("Outer Cut Off", &outerCutOff, 0.01, 0.1, "%.6f");

            ImGui::InputFloat("Constant", &constant);
            ImGui::InputFloat("Linear", &linear, 0.01, 0.1, "%.6f");
            ImGui::InputFloat("Quadratic", &quadratic, 0.01, 0.1, "%.6f");

            if(ImGui::Button("Set copy GameObject direction")) {
                direction = gameObject->transform.matrix.v.dir;
            }
            break;
    }

}

Light::~Light() {
//    Component::~Component();
    alllights.erase(alllights.begin()+light_id);
    for(int i = light_id; i < alllights.size(); i++)
        alllights[i]->light_id--;

    int size = alllights.size();
    API3D::instance->uniform1iv(Shader_Opengl::baseShader(), "nbLights", &size, 1);
}

nlohmann::json Light::toJson() {
    nlohmann::json json = Component::toJson();

    json["type"] = type;

    json["direction"] = {direction.x, direction.y, direction.z};
    json["position"] = {position.x, position.y, position.z};
    json["color"] = {color.x, color.y, color.z};

    json["cutOff"] = cutOff;
    json["outerCutOff"] = outerCutOff;

    json["constant"] = constant;
    json["linear"] = linear;
    json["quadratic"] = quadratic;

    return json;
}

void Light::parseJson(nlohmann::json json) {
    Component::parseJson(json);

    type = json["type"];

    direction.x = json["direction"][0];
    direction.y = json["direction"][1];
    direction.z = json["direction"][2];

    position.x = json["position"][0];
    position.y = json["position"][1];
    position.z = json["position"][2];

    color.x = json["color"][0];
    color.y = json["color"][1];
    color.z = json["color"][2];

    cutOff = json["cutOff"];
    outerCutOff = json["outerCutOff"];

    constant = json["constant"];
    linear = json["linear"];
    quadratic = json["quadratic"];
    quadratic = json["quadratic"];
}
