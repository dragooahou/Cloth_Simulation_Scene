//
// Created by etien on 04/02/2021.
//

#ifndef CLOTHSIM_LIGHT_H
#define CLOTHSIM_LIGHT_H

#include <glm/vec3.hpp>
#include <Component.h>
#include "RenderingChain.h"

struct Light : public Component {

    static std::vector<Light*> alllights;

    enum Type {
        Direction = 0,
        Point = 1,
        Spot = 2
    } type = Point;

    std::vector<std::string> typenames = {"Direction", "Point", "Spot"};

    Light();

    ~Light() override;

    int light_id;

    Vec3 direction = Vec3::DOWN;
    Vec3 position = {0, 0, 0};

    float cutOff;
    float outerCutOff;

    float constant = 1.0f;
    float linear = 0.22f;
    float quadratic = 0.20f;

    Vec3 color = {2, 2, 2};

    void render(RenderingChain* renderingChain) override;

    void renderGUI() override;

    nlohmann::json toJson() override;

    void parseJson(nlohmann::json json) override;
};

#endif //CLOTHSIM_LIGHT_H
