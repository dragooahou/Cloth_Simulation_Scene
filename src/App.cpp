#include <ImGui/impl/imgui_impl_glfw.h>
#include <API/OpenGL/API3D_Opengl.h>
#include "App.hpp"

API3D* API3D::instance;

///////////////////////////////////////////////////////////////
// méthodes publiques
///////////////////////////////////////////////////////////////
App::App() :
        m_isRunning(true),
        m_actualStep(Steps::LOAD_RESSOURCES),


        m_loadingMenu(window),
//    m_mainMenu(window),
    m_scene(window)

{
    thisInstance=this;
}

void App::init(int argc, char* argv[])
{
    this->initWindow(argc, argv);
    m_th_initSubClass = std::async(std::launch::async, &App::initSubClass, this);
}
void App::run()
{

    //Commandes pour init ImGUi ( a mettre juste avant la mainloop)
    ImGui::CreateContext();
    ImGui::StyleColorsClassic();

    API3D::instance->ImGuiInit(window);

    // Main loop
    while(!glfwWindowShouldClose(window)) {

        update();

        if(!iconified)
            render();

        glfwPollEvents();

    }

    // Fin du programme
}


///////////////////////////////////////////////////////////////
// méthodes d'initialisations
///////////////////////////////////////////////////////////////


void App::initWindow(int argc, char* argv[])
{
    glfwSetErrorCallback([](int error_code, const char* description) {
        std::cerr << "glfw error " << error_code << ": " << description << std::endl;
    });


    if (!glfwInit()) {
        std::cerr << "Failed to initialize glfw" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CLIENT_API, API3D::instance->getClientAPI());

//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (!window){
        glfwTerminate();
        std::cerr << "Failed to create window" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);


    API3D::instance->initialize(window);

    ///////////////////////////////////////////////////////////////
    // callback
    glfwSetWindowSizeCallback(window, resizeFunction);
    glfwSetFramebufferSizeCallback(window, resizeFunction);
    glfwSetKeyCallback(window, keyFunction);
    glfwSetMouseButtonCallback(window, mouseButtonFunction);
    glfwSetCursorPosCallback(window, cursorPosFunction);
    glfwSetScrollCallback(window, scrollFunction);

    glfwSetWindowIconifyCallback(window, [](GLFWwindow* win, int state) {
        App::thisInstance->iconified = state;
    });

//    API3D::instance->printWorkGroupsCapabilities();

//    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

}

void App::initSubClass()
{
    m_loadingMenu.init();
//    m_mainMenu.init();
    m_scene.init();
    // on indique que les ressources sont chargés
    m_actualStep = Steps::FRACTAL_SCENE;
}

///////////////////////////////////////////////////////////////
// Méthodes d'exectutions
///////////////////////////////////////////////////////////////

void App::event()
{
    Event e;
    while (pollEvents(e))
    {
        switch (m_actualStep)
        {
        case LOAD_RESSOURCES:
            m_loadingMenu.event(e);
            break;
        case MAIN_MENU:
//            m_mainMenu.event(e);
            break;
        case FRACTAL_SCENE:
            m_scene.event(e);
            break;
        
        default:
            break;
        }
    }
    
   
}

void App::update()
{
    event();

    switch (m_actualStep)
    {
    case LOAD_RESSOURCES:
        m_actualStep = m_loadingMenu.update();
        break;
    case MAIN_MENU:
//        m_actualStep = m_mainMenu.update();
        break;
    case FRACTAL_SCENE:
        m_actualStep = m_scene.update();
        break;
    }
}



void App::render()
{
    //Fonctions clées pour ImGui
    API3D::instance->ImGuiNewFrame();
    ImGui_ImplGlfw_NewFrame();

    API3D::instance->clearScreen();

    switch (m_actualStep)
    {
    case LOAD_RESSOURCES:
        m_loadingMenu.render();
        break;
    case MAIN_MENU:
//        m_mainMenu.render();
        break;
    case FRACTAL_SCENE:
        m_scene.render();
        break;
    }


    ImGui::Render();
    API3D::instance->ImGuiRenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);

}

///////////////////////////////////////////////////////////////
// méthodes de gestion de evenements
///////////////////////////////////////////////////////////////

bool App::pollEvents(Event &e)
{
    if (m_eventList.size() == 0) return false;
    e = m_eventList.back();
    m_eventList.pop_back();
    return true;
}

void App::resize(int width, int height)
{
    m_width = width;
    m_height = height;
    API3D::instance->resizeScreen(width, height);
    Camera::aspect = (float)width / (float)height;
}

void App::keyPressed(unsigned char key, int x, int y)
{
    Event e;
    e.initKeyPress(key, x, y);
    m_eventList.push_back(e);
}
void App::keyReleased(unsigned char key, int x, int y)
{
    Event e;
    e.initKeyReleased(key, x, y);
    m_eventList.push_back(e);
}
void App::specialkeyPressed(int key, int x, int y)
{
    Event e;
    e.initSpecialKeyPress(key, x, y);
    m_eventList.push_back(e);
}
void App::specialkeyReleased(int key, int x, int y)
{
    Event e;
    e.initSpecialKeyReleased(key, x, y);
    m_eventList.push_back(e);
}

void App::mouseButton(int button, int state, int x, int y)
{
    Event e;
    e.initMouseButton(button, state, x, y);
    m_eventList.push_back(e);
}

void App::mouseMove(int x, int y)
{
    Event e;
    e.initMouseMove(x, y);
    m_eventList.push_back(e);
}

///////////////////////////////////////////////////////////////
// Méthodes de callback
///////////////////////////////////////////////////////////////

void App::resizeFunction(GLFWwindow* win, int width, int hight) {
    App::thisInstance->resize(width, hight);
}

void App::mouseButtonFunction(int button, int state, int x, int y) {
    App::thisInstance->mouseButton(button, state, x, y);
}

void App::cursorPosFunction(GLFWwindow* win, double x, double y) {
    App::thisInstance->mouseMove(x, y);
}


///////////////////////////////////////////////////////////////
// Instanciation des varaibles statiques
///////////////////////////////////////////////////////////////
App* App::thisInstance = nullptr;

void App::keyFunction(GLFWwindow *win, int key, int scancode, int action, int mod) {

    double xpos, ypos;
    glfwGetCursorPos(win, &xpos, &ypos);

    // TODO wesh je sais pas quoi faire là
    switch (action) {
        case GLFW_PRESS:
            App::thisInstance->keyPressed(key, xpos, ypos);
            break;

        case GLFW_RELEASE:
            App::thisInstance->keyReleased(key, xpos, ypos);
            break;
    }

}

void App::mouseButtonFunction(GLFWwindow *win, int button, int action, int mod) {
    double xpos, ypos;
    glfwGetCursorPos(win, &xpos, &ypos);

    App::thisInstance->mouseButton(button, action, xpos, ypos);
}

void App::scrollFunction(GLFWwindow *win, double xoffset, double yoffset) {
    double xpos, ypos;
    glfwGetCursorPos(win, &xpos, &ypos);

    // FIXME jpp c'est degueulasse
    App::thisInstance->mouseButton(yoffset < 0 ? 4 : 3, 0, xpos, ypos);
}

void App::cleanup() {
    API3D::instance->cleanup();
    glfwDestroyWindow(window);
    glfwTerminate();
}
