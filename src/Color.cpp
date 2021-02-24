#include "Color.hpp"

Color::Color():
    r(0),g(0),b(0),a(0)
{

}

Color::Color(const unsigned char& r,const unsigned char& g,const unsigned char& b, const unsigned char& a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}


std::ostream &Color::operator<<(std::ostream &out) 
{
    out <<"Color("<<(unsigned int) this->r<<", "<<(unsigned int)this->g<<", "<<(unsigned int)this->b<<", "<<(unsigned int)this->a <<")";
    return out;
}