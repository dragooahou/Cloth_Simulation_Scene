#pragma once
#ifndef APP_HPP
#define APP_HPP

// includes externes au projet
#include <iostream>
#include <future>
#include <list>

// includes OPEN_GL
#include <GLFW/glfw3.h>
#include <API/API3D.h>

// includes internes au projet
#include "Event.hpp"
#include "LoadingMenu.hpp"
#include "MainMenu.hpp"
#include "Scene.hpp"




///////////////////////////////////////////////////////////////////
// class mère de l'application
//
// cette class s'occupe de gèrer la fenètre principale
// et de corrdoner les differants composants de l'application

class App
{
    ///////////////////////////////////////////////////////////////
    // méthodes publiques
    ///////////////////////////////////////////////////////////////
    
public:
    ///////////////////////////////////////////////////////////////
    // constructeur de l'application
    // initialise les atributs
    App();

    ///////////////////////////////////////////////////////////////
    // methode permettant le lancement d'autres méthodes d'initialisation
    // comme par exemple loadTexture() ou loadSounds();
    // /!\ cette methode appel seulement d'autres methodes /!\ /*
    //
    // RMQ : cette methode est séparée du constructeur pour :
    // 1 - une meilleur organisation / découpage
    // 2 - pouvoir appeler cette méthode dans un thread si un chargement
    //     de textures est nécéssaire
    void init(int argc, char* argv[]);

    ///////////////////////////////////////////////////////////////
    // methode de lancement de l'application
    // -> lance la boucle principale de l'app
    void run();

    ///////////////////////////////////////////////////////////////
    // nettoyage des ressources
    void cleanup();

private:
    ///////////////////////////////////////////////////////////////
    // méthodes d'initialisations
    ///////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////////
    // Création de la fenêtre
    void initWindow(int argc, char* argv[]);

    ///////////////////////////////////////////////////////////////
    // intialisation des sous-classes (exectuée dans un thread)
    void initSubClass();

    ///////////////////////////////////////////////////////////////
    // Méthodes d'exectutions
    ///////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////
    // methode permettant la gestion des evenements
    // elle s'ocupe d'informer aux sous-classes des evenement
    // qui ont eu lieu sur l'applciation
    void event();

    ///////////////////////////////////////////////////////////////
    // methode effectuant les calculs 
    void update();

    ///////////////////////////////////////////////////////////////
    // méthode effectuant l'affichage
    void render();

    ///////////////////////////////////////////////////////////////
    // méthodes de gestion de evenements
    ///////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////
    // méthode de redimentionnement
    void resize(int width, int height);

    ///////////////////////////////////////////////////////////////
    // méthodes de parcour des events
    bool pollEvents(Event &e);

    ///////////////////////////////////////////////////////////////
    // méthode récupèrant les evenement de type key pressed
    void keyPressed(unsigned char key, int x, int y); 
   
    ///////////////////////////////////////////////////////////////
    // méthode récupèrant les evenement de type key pressed
    void keyReleased(unsigned char key, int x, int y);

    ///////////////////////////////////////////////////////////////
    // méthode récupèrant les evenement de type key pressed
    void specialkeyPressed(int key, int x, int y); 
   
    ///////////////////////////////////////////////////////////////
    // méthode récupèrant les evenement de type key pressed
    void specialkeyReleased(int key, int x, int y);

    ///////////////////////////////////////////////////////////////
    // méthode récupèreant les evenement sur les boutons
    // de la souris
    void mouseButton(int button, int state, int x, int y);

    ///////////////////////////////////////////////////////////////
    // méthode récupèreant les evenement sur les
    // déplacements de la souris
    void mouseMove(int x, int y);

    ///////////////////////////////////////////////////////////////
    // Méthodes de callback
    ///////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////
    // fonction de rendu appelant la méthode resize
    // 
    static void resizeFunction(GLFWwindow* win, int width, int hight);


    ///////////////////////////////////////////////////////////////
    // fonction permettant de récupèrer les eventments de type
    // mouse click
    static void mouseButtonFunction(int button, int state, int x, int y);

    ///////////////////////////////////////////////////////////////
    // fonction permettant de récupèrer les eventments de type
    // mouse move
    static void cursorPosFunction(GLFWwindow* win, double x, double y);

    static void keyFunction(GLFWwindow* win, int key, int scancode, int action, int mod);
    static void mouseButtonFunction(GLFWwindow* win, int button, int action, int mod);
    static void scrollFunction(GLFWwindow* win, double xoffset, double yoffset);


    ///////////////////////////////////////////////////////////////
    // attributs
    ///////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////
    // la fenètre
    GLFWwindow* window;

    ///////////////////////////////////////////////////////////////
    // dimmentions de la fenêtre
    int m_width;
    int m_height;

    ///////////////////////////////////////////////////////////////
    // liste des évenements de la fenre
    std::list<Event> m_eventList;

    ///////////////////////////////////////////////////////////////
    // etapes d'executions
    Steps m_actualStep;

    ///////////////////////////////////////////////////////////////
    // true -> l'application tourne
    // false -> l'application est terminée !
    bool m_isRunning;

    ///////////////////////////////////////////////////////////////
    // sous class gérant le menu de chargement en attendant
    // que les ressources se chargent en mémoire
    LoadingMenu m_loadingMenu;

    ///////////////////////////////////////////////////////////////
    // sous class gérant le menu principal de l'application
//    MainMenu m_mainMenu;

    ///////////////////////////////////////////////////////////////
    // sous class gérant l'affichage d'une fractale
    Scene m_scene;

    ///////////////////////////////////////////////////////////////
    // threads
    ///////////////////////////////////////////////////////////////
    std::future<void> m_th_initSubClass;

    ///////////////////////////////////////////////////////////////
    // Attributs statiques
    ///////////////////////////////////////////////////////////////
    
    ///////////////////////////////////////////////////////////////
    // l'instance de this pour pouvoir avoir accès a l'instance
    // de la class App dans des méthodes statique
    // on fait ceci car les méthodes de callback appelés par GLFW
    // doivent être des fonctions et non des méthodes
    static App* thisInstance;

    ///////////////////////////////////////////////////////////////
    // Constantes
    ///////////////////////////////////////////////////////////////
    const int INIT_WINDOW_WIDTH = 800;
    const int INIT_WINDOW_HEIGHT = 600;
    const char* WINDOW_TITLE = "Ho yeah babe";

    bool iconified = false;
};


#endif