//
// Created by Alarm on 05/11/2020.
//

#include <ImGui/imgui.h>
#include <API/API3D.h>
#include "GUI/DebugWindow.h"
#include "Simulation/Cloth.h"


DebugWindow::DebugWindow() {

}

void DebugWindow::draw(Camera* camera) {
    if(!p_p_open)return;
    if(ImGui::Begin("Debug",&p_p_open,ImGuiWindowFlags_AlwaysAutoResize)){
        ig::Text("%s", API3D::instance->toString().c_str());

        int maxTextureSize;
//        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
        ig::Text("Max texture size : %d", maxTextureSize);

        // Aficheur de fps
        ig::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ig::GetIO().Framerate, ig::GetIO().Framerate);
        ig::SameLine();

        if(ig::Checkbox("V-sync", &vsync)) {
            glfwSwapInterval(vsync);
        }

        // Fill an array of contiguous float values to plot
        // Tip: If your float aren't contiguous but part of a structure, you can pass a pointer to your first float
        // and the sizeof() of your structure in the "stride" parameter.
        if (m_refreshTime == 0.0)
            m_refreshTime = ImGui::GetTime();
        while (m_refreshTime < ImGui::GetTime()) // Create data at fixed 60 Hz rate for the demo
        {
            float val = ImGui::GetIO().Framerate;
            m_max = val > m_max ? val : m_max;
            if(m_fpsValues[m_valuesOffset] == m_max) {
                m_fpsValues[m_valuesOffset] = val;
                m_max = 0;
                for(int i = 0; i < IM_ARRAYSIZE(m_fpsValues); i++)
                    m_max = m_fpsValues[i] > m_max ? m_fpsValues[i] : m_max;
            }
            m_fpsValues[m_valuesOffset] = val;
            m_valuesOffset = (m_valuesOffset + 1) % IM_ARRAYSIZE(m_fpsValues);
            m_refreshTime += 1.0f / 3.0f;
        }

        ImGui::PlotLines("##Lines", m_fpsValues, IM_ARRAYSIZE(m_fpsValues), m_valuesOffset, NULL, 0, m_max*1.3f, ImVec2(0, 80.0f));


        {
            ImGui::Checkbox("Cloth use GPU", &Cloth::useGPU);


        }
    }
    ImGui::End();
}
