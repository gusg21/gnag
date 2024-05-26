//
// Created by Angus Goucher on 5/25/2024.
//

#include "gnagtool.h"
#include "SDL_image.h"

GnagTool::GnagTool(SDL_Renderer *renderer) : m_Renderer(renderer) {
    m_GUIs.push_back(new FileExplorer(this));

    m_EmptyTileTexture = IMG_LoadTexture(renderer, "gfx/emptytile.png");
    m_SpikesTileTexture = IMG_LoadTexture(renderer, "gfx/spikes.png");
    m_QuestionTileTexture = IMG_LoadTexture(renderer, "gfx/questiontile.png");
}

void GnagTool::DoGUI() {
    for (uint32_t guiIndex = 0; guiIndex < m_GUIs.size(); guiIndex++) {
        m_GUIs[guiIndex]->DoGUI();
    }
    InternalGUI();

    for (uint32_t guiIndex = 0; guiIndex < m_GUIsToClose.size(); guiIndex++) {
        auto guiIter = std::find(m_GUIs.begin(), m_GUIs.end(), m_GUIsToClose[guiIndex]);
        if (guiIter != m_GUIs.end()) {
            m_GUIs.erase(guiIter);
        }
    }
    m_GUIsToClose.clear();
}

void GnagTool::InternalGUI() {
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y));
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, m_MenuBarHeight));
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::Begin("All-powerful GnagTool", NULL,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse
                 | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNavFocus |
                 ImGuiWindowFlags_NoDecoration);
    {
        m_MenuBarHeight = ImGui::GetWindowHeight();

        ImGui::Columns(10);
        ImGui::SetColumnWidth(0, 125);
        {
            if (ImGui::Button("Run Buildinator")) {
                GnagOSWrapper::RunTheBuildinator();
            }
        }
        ImGui::NextColumn();
        ImGui::SetColumnWidth(1, 100);
        { ImGui::Text("FPS: %.2f", m_FPS); }
        ImGui::NextColumn();
        ImGui::SetColumnWidth(2, 300);
        {
            std::string gnagPath;
            if (GnagOSWrapper::GetGnagPath(gnagPath)) {
                ImGui::Text("Gnag Path: %s", gnagPath.c_str());
            } else {
                ImGui::Text("Gnag Path: unset");
            }
        }
    }
    ImGui::End();
}

void GnagTool::Update(float deltaTime) {
    for (uint32_t guiIndex = 0; guiIndex < m_GUIs.size(); guiIndex++) {
        m_GUIs[guiIndex]->Update(deltaTime);
    }

    m_FPS = 1.f / deltaTime;
    m_DeltaTime = deltaTime;
}

void GnagTool::AddGUI(ToolGUI *gui) {
    m_GUIs.push_back(gui);
}

void GnagTool::CloseGUI(ToolGUI *gui) {
    m_GUIsToClose.push_back(gui);
}
