//
// Created by etien on 04/02/2021.
//

#include <utility>

#include "Rendering/RenderingChain.h"

void RenderingChain::render(Camera *camera) {
    while (!steps.empty()) {
        steps.front().renderFunction(camera);
        steps.pop_front();
    }
}

void RenderingChain::addStep(const RenderFunction& step) {
    steps.emplace_back(step);
}

RenderingChain::RenderStep::RenderStep(RenderingChain::RenderFunction renderFunction) : renderFunction(std::move(renderFunction)) {}
