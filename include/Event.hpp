#pragma once
#ifndef EVENT_HPP
#define EVENT_HPP

// includes externes au projet
#include <iostream>

// includes internes au projet
#include "Keyboard.hpp"
#include "MouseButton.hpp"

///////////////////////////////////////////////////////////////
// class générale de gestion des evenement
//
class Event
{
public:
    ///////////////////////////////////////////////////////////////
    // constructeur
    Event();

    ///////////////////////////////////////////////////////////////
    // fonction d'initialisation /!\ ne pas utiliser /!\ /
    ///////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////
    // initialisation dans le cadre d'un appuis sur touche
    void initKeyPress(const unsigned char& key, const int& x, const int& y);

    ///////////////////////////////////////////////////////////////
    // initialisation dans le cadre d'un relachement de touche
    void initKeyReleased(const unsigned char& key, const int& x, const int& y);

    ///////////////////////////////////////////////////////////////
    // initialisation dans le cadre d'un appuis sur touche spécifique
    void initSpecialKeyPress(const int& key, const int& x, const int& y);

    ///////////////////////////////////////////////////////////////
    // initialisation dans le cadre d'un relachement de touche spécifique
    void initSpecialKeyReleased(const int& key, const int& x, const int& y);
    
    ///////////////////////////////////////////////////////////////
    // initialisation dans le cadre d'un evenement
    // sur un bouton de souris
    void initMouseButton(const int& button,  const int& state, const int& x, const int& y);
    
    ///////////////////////////////////////////////////////////////
    // initialisation dans le cadre d'un evenement
    // de déplacement de souris
    void initMouseMove(const int& x, const int& y);


    ///////////////////////////////////////////////////////////////
    // Getters
    ///////////////////////////////////////////////////////////////
    
    bool isKeyboardEvent() const;
    bool isKeyPressedEvent() const;
    bool isKeyReleasedEvent() const;

    bool isMouseEvent() const;
    bool isMouseButtonPressedEvent() const;
    bool isMouseButtonReleasedEvent() const;
    bool isMouseMoveEvent() const;
    bool isMousewheelEvent() const;

    static bool MouseLeftPressed();
    static bool MouseRightPressed();
    static bool MouseWheelPressed();

    ///////////////////////////////////////////////////////////////
    // recupèration de la coordonée x de la souris pour cet evenement
    int getMouseX() const;

    ///////////////////////////////////////////////////////////////
    // recupèration de la coordonée y de la souris pour cet evenement
    int getMouseY() const;

    ///////////////////////////////////////////////////////////////
    // recupèration du déplacement de la molette
    int getMouseWeelDelta() const;

    ///////////////////////////////////////////////////////////////
    // recupèration de la touche conscerné par l'evnement
    Key getKey() const;

    ///////////////////////////////////////////////////////////////
    // recupèration du bouton de souris conscerné par l'evnement 
    MouseButton getMouseButton() const;

    ///////////////////////////////////////////////////////////////
    // METHODES STATIQUES
    ///////////////////////////////////////////////////////////////



    unsigned char m_keyid;
private:



    ///////////////////////////////////////////////////////////////
    // convertis un id glut vers la structure key
    static Key convertKeyId(const unsigned char& key);

    ///////////////////////////////////////////////////////////////
    // convertis un id glut vers la structure key
    // (pour les touches spécifiques)
    static Key convertspecialKeyId(const int& key);

    ///////////////////////////////////////////////////////////////
    // donnée compémentaires
    ///////////////////////////////////////////////////////////////
    
    bool m_isKeyboardEvent = false;
    bool m_isKeyPressedEvent = false;
    bool m_isKeyReleasedEvent = false;
    Key m_key = Key::Unknown;


    bool m_isMouseEvent = false;
    bool m_isMouseButtonPressedEvent = false;
    bool m_isMouseButtonReleasedEvent = false;
    bool m_isMouseMoveEvent = false;
    bool m_isMousewheelEvent = false;
    MouseButton m_button = MouseButton::Unknown;
    int m_xmouse;
    int m_ymouse;
    int m_weelDelta = 0;
    
    static bool m_leftPressed;
    static bool m_rightPressed;
    static bool m_middlePressed;

};

#endif