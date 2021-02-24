//
// Created by Alarm on 05/11/2020.
//

#ifndef PROJET_LSYSTEM_GUIWINDOW_H
#define PROJET_LSYSTEM_GUIWINDOW_H

#define ig ImGui

class Camera;

class GuiWindow {

public:

    GuiWindow():p_p_open(true){};

    bool* getPopen(){return &p_p_open;}
    virtual void draw(Camera* camera){};

protected:
    bool p_p_open;
};


#endif //PROJET_LSYSTEM_GUIWINDOW_H
