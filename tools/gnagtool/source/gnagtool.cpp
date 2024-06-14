//
// Created by Angus Goucher on 5/25/2024.
//

#include "gnagtool.h"

#include <SDL.h>
#include <SDL_image.h>

#include "uilayouteditor.h"

GnagTool::GnagTool(SDL_Renderer* renderer)
    : m_Renderer(renderer) {
    m_GUIs.push_back(new FileExplorer(this, GetNextGUIID()));

    std::string gnagPath;
    GnagOSWrapper::GetGnagPath(gnagPath);
    m_Sprites = new SDLT3S(renderer, gnagPath + "gfx/sprites.t3s");
    m_UISprites = new SDLT3S(renderer, gnagPath + "gfx/uisprites.t3s");
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
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y));
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, m_MenuBarHeight));
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::Begin("All-powerful GnagTool", NULL,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNavFocus |
                 ImGuiWindowFlags_NoDecoration);
    {
        m_MenuBarHeight = ImGui::GetWindowHeight();

        ImGui::BeginDisabled(GnagOSWrapper::IsTheBuildinatorBuildinating());
        if (ImGui::Button("Buildinator")) {
            if (m_RebuildBuildinator) {
                GnagOSWrapper::RunTheBuildinatorCleaner();
            }
            GnagOSWrapper::RunTheBuildinator();
        }
        ImGui::SameLine();
        ImGui::Checkbox("Rebuild", &m_RebuildBuildinator);
        ImGui::SameLine();
        if (ImGui::Button("Address Finder")) {
            GnagOSWrapper::RunTheAddressFinder();
        }
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::Checkbox("ImGui Demo", &m_ShowImGuiDemo);
        ImGui::SameLine();
        ImGui::Bullet();
        ImGui::Text("FPS: %.2f", m_FPS);
        ImGui::SameLine();
        ImGui::Bullet();
        ImGui::Text("Info");
        if (ImGui::BeginItemTooltip()) {
            std::string gnagPath;
            if (GnagOSWrapper::GetGnagPath(gnagPath)) {
                ImGui::Text("Gnag Path: %s", gnagPath.c_str());
            } else {
                ImGui::Text("Gnag Path: unset");
            }
            ImGui::EndTooltip();
        }
    }

    ImGui::End();

    if (m_ShowImGuiDemo)
        ImGui::ShowDemoWindow();
}

void GnagTool::Update(float deltaTime) {
    for (uint32_t guiIndex = 0; guiIndex < m_GUIs.size(); guiIndex++) {
        m_GUIs[guiIndex]->Update(deltaTime);
    }

    m_FPS = 1.f / deltaTime;
    m_DeltaTime = deltaTime;
}

void GnagTool::AddGUI(ToolGUI* gui) {
    m_GUIs.push_back(gui);
}

void GnagTool::CloseGUI(ToolGUI* gui) {
    m_GUIsToClose.push_back(gui);
}
