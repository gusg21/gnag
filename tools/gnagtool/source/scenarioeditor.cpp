//
// Created by Angus Goucher on 5/25/2024.
//

#include <cstdio>
#include "scenarioeditor.h"
#include "imgui.h"

ScenarioEditor::ScenarioEditor(GnagTool *gnagTool, SDL_Renderer *renderer, Scenario scenario) : ToolGUI(gnagTool), m_Scenario(scenario) {
    m_GnagTool = gnagTool;
    m_Renderer = renderer;

    m_EditorTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480);

    int centerX, centerY;
    GetWorldPosFromTilePos(scenario.GridWidth / 2, scenario.GridHeight / 2, &centerX, &centerY);
    m_ViewX = (float)centerX;
    m_ViewY = (float)centerY;
}

void ScenarioEditor::DoGUI() {
    if (ImGui::Begin("Scenario Editor"))
    {
        ImGui::Button("Test");

        ImVec2 availSize = ImGui::GetContentRegionAvail();
        RenderEditorToTexture(&m_EditorTexture, availSize);
        ImGui::Image(m_EditorTexture, availSize, ImVec2{0, 0}, ImVec2{1, 1});

        m_Focused = ImGui::IsWindowFocused();
    }
    ImGui::End();
}

void ScenarioEditor::RenderEditorToTexture(SDL_Texture** texture, ImVec2 editorSize) {
    if ((int)m_LastFrameEditorSize.x != (int)editorSize.x || (int)m_LastFrameEditorSize.y != (int)editorSize.y) {
        SDL_DestroyTexture(*texture);
        *texture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)editorSize.x, (int)editorSize.y);
    }

    SDL_SetRenderTarget(m_Renderer, *texture);
    SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 0);
    SDL_RenderClear(m_Renderer);
    {
        DrawGrid();
    }
    SDL_RenderPresent(m_Renderer);

    SDL_SetRenderTarget(m_Renderer, nullptr);

    m_LastFrameEditorSize = editorSize;

    m_ViewWidth = editorSize.x;
    m_ViewHeight = editorSize.y;
}

void ScenarioEditor::DrawGrid() {
    SDL_SetRenderDrawColor(m_Renderer, 200, 10, 10, 255);
    for (int xx = 0; xx < m_Scenario.GridWidth; xx++) {
        for (int yy = 0; yy < m_Scenario.GridHeight; yy++) {
            int worldX, worldY;
            GetWorldPosFromTilePos(xx, yy, &worldX, &worldY);
            worldX += (int)(-m_ViewX + (m_ViewWidth / 2.f));
            worldY += (int)(-m_ViewY + (m_ViewHeight / 2.f));
            SDL_Point gridTilePoints[5] = {
                    {worldX - m_TileWidth / 2, worldY},
                    {worldX, worldY - m_TileHeight / 2},
                    {worldX + m_TileWidth / 2, worldY},
                    {worldX, worldY + m_TileHeight / 2},
                    {worldX - m_TileWidth / 2, worldY}
            };
            SDL_RenderDrawLines(m_Renderer, gridTilePoints, 5);
        }
    }
}

void ScenarioEditor::Update(float deltaTime) {
    const uint8_t* keyboard = SDL_GetKeyboardState(NULL);
    if (m_Focused) {
        if (keyboard[SDL_SCANCODE_A]) {
            m_ViewX -= m_ViewPanSpeed * deltaTime;
        }
        if (keyboard[SDL_SCANCODE_D]) {
            m_ViewX += m_ViewPanSpeed * deltaTime;
        }
        if (keyboard[SDL_SCANCODE_W]) {
            m_ViewY -= m_ViewPanSpeed * deltaTime;
        }
        if (keyboard[SDL_SCANCODE_S]) {
            m_ViewY += m_ViewPanSpeed * deltaTime;
        }
    }

    ToolGUI::Update(deltaTime);
}

void ScenarioEditor::GetWorldPosFromTilePos(int tileX, int tileY, int *worldX, int *worldY) const {
    *worldX = ((tileX - tileY) * m_TileWidth / 2);
    *worldY = ((tileX + tileY) * m_TileHeight / 2);
}
