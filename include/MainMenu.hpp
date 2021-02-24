#pragma once
#ifndef MAINMENU_HPP
#define MAINMENU_HPP

// includes externes au projet
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

// includes internes au projet
#include "Steps.hpp"
#include "Event.hpp"
#include "App.hpp"

///////////////////////////////////////////////////////////////////
// sous-class de l'application
//
// cette class s'occupe de gèrer le menu d'arrivé dans l'application
class MainMenu {

public:
    ///////////////////////////////////////////////////////////////
    // contructeur par défaut (ne doit pas être appeler)
    MainMenu();

    ///////////////////////////////////////////////////////////////
    // constructeur de la class, permet de récupèrer la fenètre
    MainMenu(GLFWwindow * win);

    ///////////////////////////////////////////////////////////////
    // méthodes permettant d'initialiser des paramètres long
    // à initialisé (thread)
    void init();

    ///////////////////////////////////////////////////////////////
    // méthodes de traitements
    ///////////////////////////////////////////////////////////////
    
    ///////////////////////////////////////////////////////////////
    // gestion des evenement
    void event(const Event& e);

    ///////////////////////////////////////////////////////////////
    // gestion des calculs/ traitements
    // -> retourne l'étape à la class mère (permet le changement d'étape)
    Steps update();

    ///////////////////////////////////////////////////////////////
    // gestion de l'affichage des éléments
    void render();


private:
    ///////////////////////////////////////////////////////////////
    // pointeur vers la fenètre
    GLFWwindow* m_win;

    ///////////////////////////////////////////////////////////////
    // boolean qui indique si le traitement de cette sous
    // class est terminé
    bool m_nextStep;

};

#endif 