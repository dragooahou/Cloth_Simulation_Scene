#include <glm/glm.hpp>
#include "Rendering/SolidSphere.h"
#include "Scene.hpp"

Scene* Scene::last_instance;

float map2(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Scene::Scene()
{
}
Scene::Scene(GLFWwindow* win):
        m_win(win),
        m_nextStep(false),
//        m_fpsText(10, 20, "0"),
        m_mainMenu()
{
//    m_fpsText.setFillColor(Color(200,50,50));
    last_instance = this;

}

///////////////////////////////////////////////////////////////
// Méthodes d'initialisation
///////////////////////////////////////////////////////////////

void Scene::init() {
    // on definit l'orientation de la caméra
    m_camera.setPosition({0, 0.0, 0.3});
    // on définit le point d'appartition de la caméra
    m_camera.setCenterPositionInstant({10, 0.03, 0});
    // on définit le point d'arrivée
    m_camera.setCenterPosition({0, 0.03, 0});

    m_mainThreadActionList.push_back([](Scene *fs) {
        TextureManager::loadAll();
        Component::init();
    });

    m_mainThreadActionList.push_back([](Scene *fs) {

//        {
//            auto go = new GameObject();
//            go->models.emplace_back("resources/Model/garage/warehouse.obj");;
//            fs->m_gameObjects.emplace_back(go);
//        }
/*        {
            auto go = new GameObject();
            auto mesh = SolidSphere(SPHERE_RADIUS, 20, 20, {0.0, 0.4, 1});
            mesh.useLight = false;

            Model model;
            model.meshes.push_back(std::move(mesh));

            go->models.emplace_back(std::move(model));
            go->transform.setPosition({1, 1, 1});
            go->name = "Sun";
            go->addComponent(new Light());

            fs->m_gameObjects.emplace_back(go);

        }*/

    });

/*    m_mainThreadActionList.push_back([](Scene *fs) {
        fs->m_grid = GameObject();
        fs->m_grid.mesh.setColor(0.4, 0.4, 0.4);
        for (float i = -25; i < 25; i += 0.1f) {
            if (i > -.001f && i < .001f) continue;
            fs->m_grid.mesh.addLine({-25, 0, i}, {25, 0, i}, Vec3::UP, Vec3::UP);
            fs->m_grid.mesh.addLine({i, 0, -25}, {i, 0, 25}, Vec3::UP, Vec3::UP);
            fs->m_grid.mesh.texcoords.push_back(-25);
            fs->m_grid.mesh.texcoords.push_back(0);
            fs->m_grid.mesh.texcoords.push_back(25);
            fs->m_grid.mesh.texcoords.push_back(0);
        }

        fs->m_grid.mesh.setColor(1, 0, 0);
        fs->m_grid.mesh.addLine({-25, 0, 0}, {25, 0, 0}, Vec3::UP, Vec3::UP);

        fs->m_grid.mesh.setColor(0, 0, 1);
        fs->m_grid.mesh.addLine({0, 0, -25}, {0, 0, 25}, Vec3::UP, Vec3::UP);
        fs->m_grid.drawingMethod = API3D::instance->getDrawingMethod(API3D::Lines);
        fs->m_grid.mesh.updateBuffers();

        fs->m_grid.useLight = false;
        fs->m_grid.useTexture = false;

    });

    m_mainThreadActionList.push_back([](Scene *fs) {
        fs->m_gameObjects.emplace_back(new Cloth(0.05,1,4,45));
        fs->m_gameObjects.back()->material.diffuse = TextureManager::loadTexture("resources/Textures/fabric1.jpg");
        fs->m_gameObjects.back()->material.specular = TextureManager::emptyTexture();
        fs->m_gameObjects.back()->transform.setPosition({0, 1, 0});
    });*/

}

///////////////////////////////////////////////////////////////
// Méthodes d'executions
///////////////////////////////////////////////////////////////

void Scene::event(const Event& e)
{

    if(e.isMouseMoveEvent()) {
        Vec3 mouse = Vec3(e.getMouseX(), e.getMouseY());
        Vec3 delta = mouse - m_pmouse;

        if (Event::MouseRightPressed()) {
            m_camera.rotateHorizontaly(delta.x * ROTATE_FACTOR);
            m_camera.rotateVertically(delta.y * ROTATE_FACTOR);
        }
        if(Event::MouseWheelPressed()) {
            m_camera.translateHorizontaly(delta.x, TRANSLATE_FACTOR);
            m_camera.translateVertically(delta.y, TRANSLATE_FACTOR);
        }

        m_pmouse.set(mouse);
    }

    else if (e.isMousewheelEvent()  && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
        if (e.getMouseWeelDelta()>0)
        {
            m_camera.zoom(1.0/ZOOM_FACTOR);
        }
        else
        {
            m_camera.zoom(ZOOM_FACTOR);
        }

    }


}

Steps Scene::update()
{

    while (m_mainThreadActionList.size() > 0) {
        m_mainThreadActionList[0](this);
        m_mainThreadActionList.erase(m_mainThreadActionList.begin());
    }



    for(auto& go : m_gameObjects)
        go->update(ig::GetIO().DeltaTime);


    return Steps::FRACTAL_SCENE;
}

void Scene::render()
{
    RenderingChain renderingChain;

    m_camera.updateMatrix();

    // region Render
    for(auto& go : m_gameObjects)
        go->render(&renderingChain);
    // endregion

    // region Render Grid
    if(m_showGrid) {
//        m_grid.render(&renderingChain);
    }
   // endregion


    renderingChain.render(&m_camera);

    API3D::instance->clearScreenColor();

    m_mainMenu.draw(&m_camera);

}

Scene::~Scene() {

}
