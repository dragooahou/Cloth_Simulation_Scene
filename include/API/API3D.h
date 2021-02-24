#ifndef PROJET_LSYSTEM_API3D_H
#define PROJET_LSYSTEM_API3D_H

#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <Rendering/Material.h>

#include "Rendering/Mesh.h"
#include "ImGui/imgui.h"

class API3D {

protected:
    API3D() = default;

public:

    enum DrawingMethod : int {
        Lines
    };

    virtual void ImGuiInit(GLFWwindow* win) = 0;
    virtual void ImGuiNewFrame() = 0;
    virtual void ImGuiRenderDrawData(ImDrawData* data) = 0;

    virtual void initialize(GLFWwindow* window) = 0;
    virtual void cleanup() = 0;
    virtual void clearScreen() = 0;
    virtual void clearScreenColor() = 0;

    virtual void useBaseShader() = 0;
    virtual int getBaseShader() = 0;
    virtual void useShader(int id) = 0;


    virtual int  getDrawingMethod(DrawingMethod m) = 0;
    virtual void resizeScreen(int width, int height) = 0;
    virtual void renderMesh(const Mesh& mesh) = 0;
    virtual void updateMeshBuffers(Mesh& mesh) = 0;
    virtual void setBasePVM(const glm::mat4 &projection, const glm::mat4& view, const glm::mat4& model) = 0;

    virtual std::string toString() = 0;

    static API3D* instance;

    virtual int getClientAPI() = 0;


    virtual void uniform3fv(int shader, const char* name, float* v, size_t count) = 0;
    virtual void uniform1fv(int shader, const char* name, float* v, size_t count) = 0;
    virtual void uniform1iv(int shader, const char* name, int* v, size_t count) = 0;

    virtual void freeMeshBuffers(Mesh mesh) = 0;

    virtual void printWorkGroupsCapabilities() = 0;
};


#endif //PROJET_LSYSTEM_API3D_H
