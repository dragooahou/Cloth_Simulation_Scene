//
// Created by Alarm on 04/11/2020.
//

#ifndef PROJET_LSYSTEM_MAINMENUBAR_H
#define PROJET_LSYSTEM_MAINMENUBAR_H

/*
 * Classe crée par: Kilian
 * But: cette classe va servir de classe mère a toutes les autres
 * interfaces, cet a dire qu'elle va instancier toutes les fenetres de l'app
 */

#include "DebugWindow.h"
#include "GuiWindow.h"
#include "EditorWindow.h"

class Camera;

class MainMenuBar {

public:

    bool m_fps_limit = 60;

    MainMenuBar();

    void draw(Camera* camera);

private:

    /*
     * Ici on aura un champ par fenetre de l'app
     */

    DebugWindow m_debug;
    EditorWindow m_editor;

    bool m_exampleWindowPopen = false;

};


#endif //PROJET_LSYSTEM_MAINMENUBAR_H
