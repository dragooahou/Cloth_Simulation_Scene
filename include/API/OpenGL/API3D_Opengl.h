#ifndef PROJET_LSYSTEM_API3D_OPENGL_H
#define PROJET_LSYSTEM_API3D_OPENGL_H

#include "API/API3D.h"

class API3D_Opengl : public API3D {

public:

    API3D_Opengl() = default;

    void ImGuiInit(GLFWwindow* win) override;
    void ImGuiNewFrame() override;
    void ImGuiRenderDrawData(ImDrawData *data) override;

    void initialize(GLFWwindow* window) override;
    void cleanup() override;
    void clearScreen() override;
    void clearScreenColor() override;
    void useBaseShader() override;
    void useShader(int id) override;
    int getDrawingMethod(DrawingMethod m) override;
    void resizeScreen(int width, int height) override;
    void renderMesh(const Mesh& mesh) override;
    void updateMeshBuffers(Mesh &mesh) override;
    void setBasePVM(const glm::mat4 &projection, const glm::mat4& view, const glm::mat4& model) override;

    int getBaseShader() override;

    std::string toString() override;

    int getClientAPI() override;

    void uniform3fv(int shader, const char *name, float *v, size_t count) override;
    void uniform1fv(int shader, const char *name, float *v, size_t count) override;
    void uniform1iv(int shader, const char *name, int *v, size_t count) override;

    void freeMeshBuffers(Mesh mesh) override;

    // Compute shaders
    void printWorkGroupsCapabilities() override;


};


#endif //PROJET_LSYSTEM_API3D_OPENGL_H
