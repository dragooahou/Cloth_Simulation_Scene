#include <API/OpenGL/API3D_Opengl.h>

#include "App.hpp"
#include "JobDispatcher.h"

int main() {

    JobDispatcher::init();

    API3D::instance = new API3D_Opengl();

    App app;
    app.init(0, 0);
    app.run();
    app.cleanup();

    JobDispatcher::stop();
}