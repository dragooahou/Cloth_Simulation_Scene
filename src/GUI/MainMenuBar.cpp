//
// Created by Alarm on 04/11/2020.
//

#include <ImGui/imgui.h>
#include <ImGui/impl/imgui_impl_opengl3.h>
#include <Scene.hpp>
#include <ImGui/ImFileDialog.h>
#include "GUI/MainMenuBar.h"
#include "JobDispatcher.h"

#define ig ImGui

void MainMenuBar::draw(Camera* camera) {

    //region menu
    if(ImGui::BeginMainMenuBar()){

        if(ImGui::BeginMenu("File")){

            if(ImGui::MenuItem("Save as...")) {
                ifd::FileDialog::Instance().Open("save_file_dialog", "Save scene", "JSON (*.json){.json}");
            }

            if(ImGui::MenuItem("Load...")) {
                ifd::FileDialog::Instance().Open("load_file_dialog", "Load scene", "JSON (*.json){.json}");
            }

            ImGui::EndMenu();
        }

        if (ifd::FileDialog::Instance().IsDone("save_file_dialog")) {
            if (ifd::FileDialog::Instance().HasResult()) {
                const std::wstring& res = ifd::FileDialog::Instance().GetResult();

                nlohmann::json json;
                for(auto& go : Scene::last_instance->m_gameObjects)
                    json["gameobjects"].push_back(go->toJson());

                std::ofstream o(std::filesystem::path(res.c_str()));
                o << std::setw(4) << json << std::endl;
            }
            ifd::FileDialog::Instance().Close();
        }

        if (ifd::FileDialog::Instance().IsDone("load_file_dialog")) {
            if (ifd::FileDialog::Instance().HasResult()) {
                const std::wstring& res = ifd::FileDialog::Instance().GetResult();

                JobDispatcher::clearJobs();

                try {
                    std::ifstream i(std::filesystem::path(res.c_str()));
                    nlohmann::json json;
                    i >> json;

                    Scene::last_instance->m_gameObjects.clear();

                    for(auto& go : json["gameobjects"]) {
                        Scene::last_instance->m_gameObjects.emplace_back(GameObject::fromJson(go));
                    }

                } catch (std::exception& e) {
                    std::cerr << "Failed to load scene : " << e.what() << std::endl;
                }

            }
            ifd::FileDialog::Instance().Close();
        }

        if(ImGui::BeginMenu("Windows")){

            ImGui::MenuItem("Debug",NULL,m_debug.getPopen());
            ImGui::MenuItem("Editor",NULL,m_editor.getPopen());

            ImGui::EndMenu();
        }

        if(ig::BeginMenu("Settings")) {
            ig::MenuItem("Show grid", NULL, &Scene::last_instance->m_showGrid);
            ig::MenuItem("Show lighting", NULL, &Scene::last_instance->m_camera.useLighting);

            ig::EndMenu();
        }


        ImGui::EndMainMenuBar();
    }
    //endregion

    //region drawFenetres
    m_debug.draw(camera);
    m_editor.draw(camera);
    //endregion
}

MainMenuBar::MainMenuBar(): m_debug() {

}
