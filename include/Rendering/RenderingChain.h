//
// Created by etien on 04/02/2021.
//

#ifndef CLOTHSIM_RENDERINGCHAIN_H
#define CLOTHSIM_RENDERINGCHAIN_H

#include <list>
#include <functional>

class Camera;

class RenderingChain {

    typedef std::function<void(Camera *)> RenderFunction;

    struct RenderStep {
        RenderStep() = delete;
        RenderStep(RenderFunction renderFunction);

        // Function called by the camera
        RenderFunction renderFunction;
    };

    std::list<RenderStep> steps;

public:
    void render(Camera* camera);
    void addStep(const RenderFunction& step);

};


#endif //CLOTHSIM_RENDERINGCHAIN_H
