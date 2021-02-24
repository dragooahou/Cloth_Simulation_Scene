//
// Created by etien on 04/02/2021.
//

#ifndef CLOTHSIM_EDITORWINDOW_H
#define CLOTHSIM_EDITORWINDOW_H


#include "GuiWindow.h"


class EditorWindow : public GuiWindow {

public:

    EditorWindow();

    void draw(Camera* camera) override;

    int currentSelectGameObjectIndex = 0;
};


#endif //CLOTHSIM_EDITORWINDOW_H
