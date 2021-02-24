#pragma once
#ifndef FRACTALSCENE_HPP
#define FRACTALSCENE_HPP

// includes externes au projet
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <GLFW/glfw3.h>

// includes internes au projet
#include "Steps.hpp"
#include "Event.hpp"
#include "Rendering/Camera.h"
#include "Rendering/RenderFunctions.h"
#include "GUI/MainMenuBar.h"
#include <Rendering/Mesh.h>
#include <Rendering/GameObject.h>
#include <API/API3D.h>
#include <Simulation/Cloth.h>
#include <Rendering/Light.h>



///////////////////////////////////////////////////////////////////
// sous-class de l'application
//
// cette class s'occupe de gèrer l'affichage d'une fractale 
// cette class est provisoire,
// elle sera sois retravaillée en profondeur ou complétement 
// refaite
class Scene {

public:
    static Scene* last_instance;

    ///////////////////////////////////////////////////////////////
    // contructeur par défaut (ne doit pas être appeler)
    Scene();

    ///////////////////////////////////////////////////////////////
    // constructeur de la class, permet de récupèrer la fenètre
    Scene(GLFWwindow* win);

    ~Scene();

    ///////////////////////////////////////////////////////////////
    // méthodes permettant d'initialiser des paramètres long
    // à initialiser (thread)
    void init();

    ///////////////////////////////////////////////////////////////
    // méthodes de traitements
    ///////////////////////////////////////////////////////////////
    
    ///////////////////////////////////////////////////////////////
    // gestion des evenement
    // -> déplacement de la vue avec le flèches
    // -> comandes autres 
    void event(const Event& e);

    ///////////////////////////////////////////////////////////////
    // gestion des calculs/ traitements
    // -> retourne l'étape à la class mère (permet le changement d'étape)
    Steps update();

    ///////////////////////////////////////////////////////////////
    // gestion de l'affichage des éléments
    void render();


    // Si on dessine la grille
    bool m_showGrid = true;

    // La liste d'objets
    std::vector<std::unique_ptr<GameObject>> m_gameObjects;


    ///////////////////////////////////////////////////////////////
// objet camera
Camera m_camera;


private:
    ///////////////////////////////////////////////////////////////
    // pointeur vers la fenètre
    GLFWwindow* m_win;

    ///////////////////////////////////////////////////////////////
    // boolean qui indique si le traitement de cette sous
    // class est terminé
    bool m_nextStep;


    //////////////////////////////////////////////////////////////
    // objet gérant les fenetres ImGui
    MainMenuBar m_mainMenu;

    // Position de la souris à la dernière frame
    Vec3 m_pmouse;

    // La grille
    GameObject m_grid;

    const double ZOOM_FACTOR = 0.8;
    const double TRANSLATE_FACTOR = 0.002;
    const double ROTATE_FACTOR = 0.5;


    // Liste des actions à faire dans le thread principal
    std::vector<void(*)(Scene* fs)> m_mainThreadActionList;
};

#endif 