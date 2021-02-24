#include <ImGui/impl/imgui_impl_opengl3.h>
#include <ImGui/impl/imgui_impl_glfw.h>
#include <sstream>
#include <iostream>
#include <GL/gl3w.h>

#include "API/OpenGL/Shader_Opengl.h"
#include "API/OpenGL/API3D_Opengl.h"

void API3D_Opengl::ImGuiInit(GLFWwindow* win) {
    ImGui_ImplGlfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL3_Init();
}

void API3D_Opengl::initialize(GLFWwindow* window) {

    // gl3w
    if (gl3wInit()) {
        fprintf(stderr, "failed to initialize OpenGL\n");
        exit(EXIT_FAILURE);
    }
    if (!gl3wIsSupported(3, 3)) {
        fprintf(stderr, "OpenGL 3.3 not supported\n");
        exit(EXIT_FAILURE);
    }
    std::cout << toString().c_str() << std::endl;

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0,1.0,1.0,0.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    // Initializing shaders
    Shader_Opengl::init();

}

void API3D_Opengl::clearScreen() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

std::string API3D_Opengl::toString() {
    std::stringstream buf;
    buf << "OpenGL " << glGetString(GL_VERSION) << ", GLSL " <<  glGetString(GL_SHADING_LANGUAGE_VERSION);
    return buf.str();
}

void API3D_Opengl::ImGuiNewFrame() {
    ImGui_ImplOpenGL3_NewFrame();
}

void API3D_Opengl::ImGuiRenderDrawData(ImDrawData *data) {
    ImGui_ImplOpenGL3_RenderDrawData(data);
}

void API3D_Opengl::resizeScreen(int width, int height) {
    glViewport(0, 0, width, height);
}

void API3D_Opengl::useBaseShader() {
    glUseProgram(Shader_Opengl::baseShader());
}

void API3D_Opengl::clearScreenColor() {
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
}

int API3D_Opengl::getDrawingMethod(DrawingMethod m) {
    switch (m) {
        case Lines:
            return GL_LINES;
    }
    return -1;
}

void API3D_Opengl::renderMesh(const Mesh& mesh) {

    int diff = 0;
    int spec = 1;
    int albedo = 2;
    int normal = 3;
    int metallic = 4;
    int roughness = 5;
    int ao = 6;
    int emission = 7;
    API3D::instance->uniform1iv(Shader_Opengl::baseShader(), "material.diffuse", &diff, 1);
    API3D::instance->uniform1iv(Shader_Opengl::baseShader(), "material.specular", &spec, 1);

    API3D::instance->uniform1iv(Shader_Opengl::baseShader(), "material.albedoMap", &albedo, 1);
    API3D::instance->uniform1iv(Shader_Opengl::baseShader(), "material.normalMap", &normal, 1);
    API3D::instance->uniform1iv(Shader_Opengl::baseShader(), "material.metallicMap", &metallic, 1);
    API3D::instance->uniform1iv(Shader_Opengl::baseShader(), "material.roughnessMap", &roughness, 1);
    API3D::instance->uniform1iv(Shader_Opengl::baseShader(), "material.aoMap", &ao, 1);
    API3D::instance->uniform1iv(Shader_Opengl::baseShader(), "material.emissionMap", &emission, 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh.material.diffuse);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mesh.material.specular);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mesh.material.albedoMap);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, mesh.material.normalMap);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, mesh.material.metallicMap);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, mesh.material.roughnessMap);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, mesh.material.aoMap);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, mesh.material.emissionMap);


    glBindVertexArray(mesh.vao);
    glDrawElements(mesh.drawingMethod, mesh.indices.size(), GL_UNSIGNED_INT, &mesh.indices[0]);
    glBindVertexArray(0);
}

void API3D_Opengl::updateMeshBuffers(Mesh &mesh) {
    if(mesh.vao == 0)
        glGenVertexArrays(1, &mesh.vao);

    if(mesh.vertexbuffer == 0)
        glGenBuffers(1, &mesh.vertexbuffer);

    if(mesh.colorbuffer == 0)
        glGenBuffers(1, &mesh.colorbuffer);

    if(mesh.normalbuffer == 0)
        glGenBuffers(1, &mesh.normalbuffer);

    if(mesh.texcoordsbuffer == 0)
        glGenBuffers(1, &mesh.texcoordsbuffer);

    glBindVertexArray(mesh.vao);

    // region vertex
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), &mesh.vertices[0], GL_STATIC_DRAW);
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            nullptr             // array buffer offset
    );
    // endregion

    // region color
    glBindBuffer(GL_ARRAY_BUFFER, mesh.colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, mesh.colors.size() * sizeof(float), &mesh.colors[0], GL_STATIC_DRAW);
    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
            1,                  // attribute 1. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            nullptr             // array buffer offset
    );
    // endregion

    // region normales
    glBindBuffer(GL_ARRAY_BUFFER, mesh.normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, mesh.normales.size() * sizeof(float), &mesh.normales[0], GL_STATIC_DRAW);
    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
            2,                  // attribute 1. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            nullptr             // array buffer offset
    );
    // endregion

    // region texcoords
    glBindBuffer(GL_ARRAY_BUFFER, mesh.texcoordsbuffer);
    glBufferData(GL_ARRAY_BUFFER, mesh.texcoords.size() * sizeof(float), &mesh.texcoords[0], GL_STATIC_DRAW);
    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(
            3,                  // attribute 1. No particular reason for 0, but must match the layout in the shader.
            2,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            nullptr             // array buffer offset
    );
    // endregion

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void API3D_Opengl::cleanup() {

}

int API3D_Opengl::getClientAPI() {
    return GLFW_OPENGL_API;
}

void API3D_Opengl::setBasePVM(const glm::mat4 &projection, const glm::mat4& view, const glm::mat4& model) {
    Shader_Opengl::setBasePVM(projection, view, model);
}

void API3D_Opengl::useShader(int id) {
    glUseProgram(id);
}

void API3D_Opengl::uniform3fv(int shader, const char *name, float *v, size_t count) {
    Shader_Opengl::uniform3fv(shader, name, v, count);
}

void API3D_Opengl::uniform1fv(int shader, const char *name, float *v, size_t count) {
    Shader_Opengl::uniform1fv(shader, name, v, count);
}

void API3D_Opengl::uniform1iv(int shader, const char *name, int *v, size_t count) {
    Shader_Opengl::uniform1iv(shader, name, v, count);
}


int API3D_Opengl::getBaseShader() {
    return Shader_Opengl::baseShader();
}

void API3D_Opengl::freeMeshBuffers(Mesh mesh) {
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(1, &mesh.vertexbuffer);
    glDeleteBuffers(1, &mesh.colorbuffer);
    glDeleteBuffers(1, &mesh.normalbuffer);
    glDeleteBuffers(1, &mesh.texcoordsbuffer);
}

void API3D_Opengl::printWorkGroupsCapabilities() {

    int workgroup_count[3];
    int workgroup_size[3];
    int workgroup_invocations;

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &workgroup_count[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &workgroup_count[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &workgroup_count[2]);

    printf ("Taille maximale des workgroups:\n\tx:%u\n\ty:%u\n\tz:%u\n",
            workgroup_size[0], workgroup_size[1], workgroup_size[2]);

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &workgroup_size[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &workgroup_size[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &workgroup_size[2]);

    printf ("Nombre maximal d'invocation locale:\n\tx:%u\n\ty:%u\n\tz:%u\n",
            workgroup_size[0], workgroup_size[1], workgroup_size[2]);

    glGetIntegerv (GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &workgroup_invocations);
    printf ("Nombre maximum d'invocation de workgroups:\n\t%u\n", workgroup_invocations);

}
