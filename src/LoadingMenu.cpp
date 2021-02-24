
#include <App.hpp>
#include "LoadingMenu.hpp"

LoadingMenu::LoadingMenu()
{

}
LoadingMenu::LoadingMenu(GLFWwindow* win):
    m_win(win)
//    m_title(200,200, "Chargement des ressources en cours ...")
{
//    m_title.setFillColor(Color(200,50,50));
}

///////////////////////////////////////////////////////////////
// Méthodes d'initialisation
///////////////////////////////////////////////////////////////

void LoadingMenu::init()
{
    
}

///////////////////////////////////////////////////////////////
// Méthodes d'exectutions
///////////////////////////////////////////////////////////////

void LoadingMenu::event(const Event& e)
{

}

Steps LoadingMenu::update()
{
    return Steps::LOAD_RESSOURCES;
}

void LoadingMenu::render()
{
//    m_title.renderMesh();
}