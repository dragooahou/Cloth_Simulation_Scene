#pragma once
#ifndef COLOR_HPP
#define COLOR_HPP

// includes externes au projet
#include <iostream>

///////////////////////////////////////////////////////////////
// class de couleurs
class Color
{

public:
    ///////////////////////////////////////////////////////////////
    // constructeur par défaut
    Color();

    ///////////////////////////////////////////////////////////////
    // constructeur
    Color(const unsigned char& r,const unsigned char& g,const unsigned char& b, const unsigned char& a = 0);
    
    ///////////////////////////////////////////////////////////////
    // opérateur d'affichage
    std::ostream &operator<<(std::ostream &out);


    unsigned char r = 0,g = 0,b = 0,a = 0;
};

 

#endif