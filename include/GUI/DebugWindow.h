//
// Created by Alarm on 05/11/2020.
//

#ifndef PROJET_LSYSTEM_DEBUGWINDOW_H
#define PROJET_LSYSTEM_DEBUGWINDOW_H

#include "GuiWindow.h"

class Camera;

class DebugWindow : public GuiWindow {

public:

    DebugWindow();

    void draw(Camera* camera);

private:

    float m_fpsValues[120] = {};
    int m_valuesOffset = 0;
    double m_refreshTime = 0.0;
    float m_max = 0.0f;

    bool vsync = 1;

};


#endif //PROJET_LSYSTEM_DEBUGWINDOW_H
