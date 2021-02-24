
#include <ImGui/imgui.h>
#include "MainMenu.hpp"

MainMenu::MainMenu()
{

}
MainMenu::MainMenu(GLFWwindow* win):
    m_win(win),
    m_nextStep(false)
//    m_title(320,200, "Menu principale"),
//    m_explainText(100, 230, "Appuillez sur F pour dessiner une superbe fractale !\n(Provisoire)")
{
//    m_title.setFillColor(Color(50,50,200));
}

///////////////////////////////////////////////////////////////
// Méthodes d'initialisation
///////////////////////////////////////////////////////////////

void MainMenu::init()
{
    
}

///////////////////////////////////////////////////////////////
// Méthodes d'executions
///////////////////////////////////////////////////////////////

void MainMenu::event(const Event& e)
{
    if (e.isKeyPressedEvent())
    {
        if (e.getKey() == Key::T)
        {
            std::cout << "T pressed !" <<std::endl;
        }
        else if (e.getKey() == Key::F)
        {
            m_nextStep = true;
        }
    }
    else if (e.isKeyReleasedEvent())
    {
        if (e.getKey() == Key::T)
        {
            std::cout << "T released" <<std::endl;
        }
    }
    else if (e.isMouseButtonPressedEvent())
    {
        if (e.getMouseButton() == MouseButton::Left)
        {
            std::cout << "Mouse Left pressed" <<std::endl;
        }
    }
    else if (e.isMouseButtonReleasedEvent())
    {
        if (e.getMouseButton() == MouseButton::Left)
        {
            std::cout << "Mouse Left released" <<std::endl;
        }
    }
    else if (e.isMousewheelEvent())
    {
        std::cout << "Mouse weel delta : "<< e.getMouseWeelDelta() <<std::endl;
        
    }
    else if (e.isMouseMoveEvent())
    {
//        std::cout << "Mouse move  !" <<std::endl;
        
    }
}

Steps MainMenu::update()
{   
    if (m_nextStep) // si on doit passer à l'étape suivante
    {
        m_nextStep = false;
        return Steps::FRACTAL_SCENE; // on ouvre la scene fractale !
    }

    return Steps::MAIN_MENU;
}

void MainMenu::render()
{
//    m_title.renderMesh();
//    m_explainText.renderMesh();
}