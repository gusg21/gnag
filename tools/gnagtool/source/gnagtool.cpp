//
// Created by Angus Goucher on 5/25/2024.
//

#include "gnagtool.h"

GnagTool::GnagTool(SDL_Renderer *renderer) {
    m_GUIs.push_back(new FileExplorer(this));
    m_GUIs.push_back(new ScenarioEditor(this, renderer, {10, 10}));
}

void GnagTool::DoGUI() {
    for (uint32_t guiIndex = 0; guiIndex < m_GUIs.size(); guiIndex++) {
        m_GUIs[guiIndex]->DoGUI();
    }
    InternalGUI();
}

void GnagTool::InternalGUI() {
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y));
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, m_MenuBarHeight));
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::Begin("All-powerful GnagTool", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse
                                                | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDecoration);
    {
        ImGui::Text("FPS: %.2f", m_FPS);
        m_MenuBarHeight = ImGui::GetWindowHeight();
    }
    ImGui::End();
}

void GnagTool::Update(float deltaTime) {
    for (uint32_t guiIndex = 0; guiIndex < m_GUIs.size(); guiIndex++) {
        m_GUIs[guiIndex]->Update(deltaTime);
    }

    m_FPS = 1.f / deltaTime;
}
