#pragma once
#ifndef LOADINGMENU_HPP
#define LOADINGMENU_HPP

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

///////////////////////////////////////////////////////////////////
// sous-class de l'application
//
// cette class s'occupe de gèrer le menu d'arrivé dans l'application
class LoadingMenu {

public:
    ///////////////////////////////////////////////////////////////
    // contructeur par défaut (ne doit pas être appeler)
    LoadingMenu();

    ///////////////////////////////////////////////////////////////
    // constructeur de la class, permet de récupèrer la fenètre
    LoadingMenu(GLFWwindow* win);

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
    // la fenètre
    GLFWwindow* m_win;

};

#endif 