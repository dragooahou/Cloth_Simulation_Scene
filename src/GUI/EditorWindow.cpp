//
// Created by etien on 04/02/2021.
//
#include <GL/gl3w.h>

#include <ImGui/imgui.h>
#include <ImGui/ImGuizmo.h>
#include <Rendering/TextureManager.h>
#include <ImGui/ImFileDialog.h>

#include "Scene.hpp"
#include "GUI/EditorWindow.h"

#define MAX_NAME_SIZE 128

EditorWindow::EditorWindow() {
    ImGuizmo::Enable(true);

    ifd::FileDialog::Instance().CreateTexture = [](uint8_t* data, int w, int h, char fmt) -> void* {
        GLuint tex;

        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, (fmt == 0) ? GL_BGRA : GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        return reinterpret_cast<void *>(tex);
    };
    ifd::FileDialog::Instance().DeleteTexture = [](void* tex) {
        GLuint texID = (unsigned long long)tex;
        glDeleteTextures(1, &texID);
    };
}

void EditTransform(Camera* camera, mymath::matrix_t& matrix)
{
    ImGuizmo::BeginFrame();
    static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
    if (ImGui::IsKeyPressed(90))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(69))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(82)) // r Key
        mCurrentGizmoOperation = ImGuizmo::SCALE;
    if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
        mCurrentGizmoOperation = ImGuizmo::SCALE;
    float matrixTranslation[3], matrixRotation[3], matrixScale[3];
    ImGuizmo::DecomposeMatrixToComponents(matrix.m16, matrixTranslation, matrixRotation, matrixScale);
    ImGui::InputFloat3("Translation", matrixTranslation);
    ImGui::InputFloat3("Rotation", matrixRotation);
    ImGui::InputFloat3("Scale", matrixScale);
    ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix.m16);

    if (mCurrentGizmoOperation != ImGuizmo::SCALE)
    {
        if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
            mCurrentGizmoMode = ImGuizmo::LOCAL;
        ImGui::SameLine();
        if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
            mCurrentGizmoMode = ImGuizmo::WORLD;
    }
    static bool useSnap(false);
    if (ImGui::IsKeyPressed(83))
        useSnap = !useSnap;
    ImGui::Checkbox("", &useSnap);
    ImGui::SameLine();
    glm::vec3 snap = { 1.f, 1.f, 1.f };
    switch (mCurrentGizmoOperation)
    {
        case ImGuizmo::TRANSLATE:
            ImGui::InputFloat3("Snap", &snap.x);
            break;
        case ImGuizmo::ROTATE:
            ImGui::InputFloat("Angle Snap", &snap.x);
            break;
        case ImGuizmo::SCALE:
            ImGui::InputFloat("Scale Snap", &snap.x);
            break;
    }
    ImGuiIO& io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    ImGuizmo::Manipulate(&camera->viewMat[0][0], &camera->projectionMat[0][0], mCurrentGizmoOperation, mCurrentGizmoMode, matrix.m16, NULL, useSnap ? &snap.x : NULL);
}

void EditorWindow::draw(Camera* camera) {
    if(!p_p_open)return;

    const char* names[Scene::last_instance->m_gameObjects.size()];
    for(int i = 0; i < Scene::last_instance->m_gameObjects.size(); i++) {
        names[i] = Scene::last_instance->m_gameObjects[i]->name.c_str();
    }

    if(ImGui::Begin("GameObjects", &p_p_open)) {
        ig::ListBox("",
                    &currentSelectGameObjectIndex,
                    names,
                    Scene::last_instance->m_gameObjects.size(),
                    Scene::last_instance->m_gameObjects.size());

        if(ig::Button("Add GameObject")) {
            Scene::last_instance->m_gameObjects.emplace_back(new GameObject());
        }

        ig::SameLine();

        if(ig::Button("Duplicate")) {
            auto go = Scene::last_instance->m_gameObjects[currentSelectGameObjectIndex].get();
            Scene::last_instance->m_gameObjects.emplace_back(GameObject::fromJson(go->toJson()));
        }

        ig::SameLine();

        if(ig::BeginPopup("Delete_popup")) {
            ig::Text("Are you sure ?");
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(200, 55, 0)));
            if(ig::Button("Yes")) {
                Scene::last_instance->m_gameObjects.erase(Scene::last_instance->m_gameObjects.begin() + currentSelectGameObjectIndex);
                ig::CloseCurrentPopup();
            }
            ig::PopStyleColor();
            ig::SameLine();
            if(ig::Button("No")) ig::CloseCurrentPopup();
            ig::EndPopup();
        }

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(200, 55, 0)));
        if(ig::Button("Delete")) {
            ig::OpenPopup("Delete_popup");
        }
        ig::PopStyleColor();


    }
    ig::End();


    if(Scene::last_instance->m_gameObjects.size() > currentSelectGameObjectIndex) {
        auto& go = Scene::last_instance->m_gameObjects[currentSelectGameObjectIndex];

        if(ImGui::Begin("Editor", &p_p_open)) {

            ig::PushID(&go);

            {
                char buf[MAX_NAME_SIZE];
                strcpy(buf, go->name.c_str());
                ig::InputText("Name", buf, MAX_NAME_SIZE);
                go->name = buf;
            }

            ig::Separator();
            ig::Text("Transform");
            EditTransform(camera , go->transform.getTransform());


            {
                ig::Separator();
                ig::Text("Models");

                int index = 0;
                for(auto& model : go->models) {

                    if(ig::CollapsingHeader(("Model " + model.name).c_str())) {
                        model.renderGUI();
                        if(ImGui::Button("Remove model")) {
                            go->models.erase(go->models.begin()+index);
                        }
                    }

                    index++;
                }


                // region open model
                if (ImGui::Button("Open a model..."))
                    ifd::FileDialog::Instance().Open("ModelOpenDialog", "Open a model", "3D Object (*.obj,*.fbx,*.dae){.obj,.fbx,.dae},.*");

                if (ifd::FileDialog::Instance().IsDone("ModelOpenDialog")) {
                    if (ifd::FileDialog::Instance().HasResult()) {
                        const std::wstring& res = ifd::FileDialog::Instance().GetResult();
                        std::string path;
                        for(char x : res) path += x;
                        go->models.emplace_back(path);
                    }
                    ifd::FileDialog::Instance().Close();
                }
                // endregion
            }

            {
                ig::Separator();
                ig::Text("Components");
                for(int i = 0; i < go->getComponents().size(); i++) {
                    auto comp = go->getComponents()[i].get();
                    ig::PushID(i);
                    if(ig::CollapsingHeader(comp->name.c_str())) {
                        comp->renderGUI();
                        if(ig::Button("Remove component")) {
                            go->removeComponent(comp);
                            i--;
                        }
                    }
                    ig::PopID();
                }


                if(ig::BeginCombo("##pourquecamarchetamerelap***", "Add component...")) {

                    for(auto [name, instanciate] : Component::getInstanciers()) {
                        bool clicked = false;
                        ig::Selectable(name.c_str(), &clicked);
                        if(clicked) go->addComponent(instanciate());
                    }

                    ig::EndCombo();
                }
            }




            ig::PopID();
        }
        ImGui::End();
    }


}
