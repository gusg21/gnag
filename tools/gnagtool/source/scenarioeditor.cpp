//
// Created by Angus Goucher on 5/25/2024.
//

#include <cstdio>

#include <vector>

#include "scenarioeditor.h"
#include "imgui.h"
#include "mouse.h"

ScenarioEditor::ScenarioEditor(GnagTool *gnagTool, SDL_Renderer *renderer, const Scenario& scenario) : ToolGUI(gnagTool), m_Scenario(scenario) {
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
        ImGui::Text("Grid Size %d x %d", m_Scenario.GridWidth, m_Scenario.GridHeight);
        ImGui::SameLine();
        ImGui::Text("View Pos %f x %f", m_ViewX, m_ViewY);

        ImVec2 availSize = ImGui::GetContentRegionAvail();
        ImVec2 screenPos = ImGui::GetCursorScreenPos();
        ImVec2 mousePos = ImGui::GetMousePos();
        m_MousePosX = mousePos.x - screenPos.x;
        m_MousePosY = mousePos.y - screenPos.y;

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
    for (int xx = 0; xx < m_Scenario.GridWidth; xx++) {
        for (int yy = 0; yy < m_Scenario.GridHeight; yy++) {
            int worldX, worldY;
            GetWorldPosFromTilePos(xx, yy, &worldX, &worldY);
            worldX += (int)(-m_ViewX + (m_ViewWidth / 2.f));
            worldY += (int)(-m_ViewY + (m_ViewHeight / 2.f));

            if (IsTileInSelectionPreview(xx, yy) && m_SelectionPreviewVisible) {
                SDL_SetRenderDrawColor(m_Renderer, 240, 150, 130, 255);
            } else if (std::find(m_Selection.begin(), m_Selection.end(), TilePos {xx, yy}) != m_Selection.end()) {
                SDL_SetRenderDrawColor(m_Renderer, 240, 230, 100, 255);
            } else {
                SDL_SetRenderDrawColor(m_Renderer, 230, 20, 10, 255);
            }

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
    const uint8_t* keyboard = SDL_GetKeyboardState(nullptr);
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

        if (Mouse::IsLeftPressed()) {
            // Selection
            int tileX, tileY;
            GetTilePosFromWorldPos(m_MousePosX + m_ViewX - (m_ViewWidth / 2.f), m_MousePosY + m_ViewY - (m_ViewHeight / 2.f), &tileX, &tileY);
            m_SelectionPreviewStart = {tileX, tileY};
            m_SelectionPreviewEnd = {tileX, tileY};
            m_SelectionPreviewVisible = true;
        }
        else if (Mouse::IsLeftDown()) {
            int tileX, tileY;
            GetTilePosFromWorldPos(m_MousePosX + m_ViewX - (m_ViewWidth / 2.f), m_MousePosY + m_ViewY - (m_ViewHeight / 2.f), &tileX, &tileY);
            m_SelectionPreviewEnd = {tileX, tileY};
            m_SelectionPreviewVisible = true;
        }
        else if (Mouse::IsLeftReleased()) {
            m_SelectionPreviewVisible = false;

            m_Selection.clear();
            int left = SDL_min(m_SelectionPreviewStart.X, m_SelectionPreviewEnd.X);
            int right = SDL_max(m_SelectionPreviewStart.X, m_SelectionPreviewEnd.X);
            int top = SDL_min(m_SelectionPreviewStart.Y, m_SelectionPreviewEnd.Y);
            int bottom = SDL_max(m_SelectionPreviewStart.Y, m_SelectionPreviewEnd.Y);
            for (int xx = left; xx <= right; ++xx) {
                for (int yy = top; yy <= bottom; ++yy) {
                    m_Selection.push_back({xx, yy});
                }
            }
        }
    }

    ToolGUI::Update(deltaTime);
}

void ScenarioEditor::GetWorldPosFromTilePos(int tileX, int tileY, int *worldX, int *worldY) const {
    int halfTileWidth = m_TileWidth / 2;
    int halfTileHeight = m_TileHeight / 2;
    *worldX = (tileY + tileX) * halfTileWidth;
    *worldY = (tileY - tileX) * halfTileHeight;
}

void ScenarioEditor::GetTilePosFromWorldPos(float worldX, float worldY, int* tileX, int* tileY) const {
    float halfTileWidth = (float)m_TileWidth / 2.f;
    float halfTileHeight = (float)m_TileHeight / 2.f;
    *tileX = (int)((float)((worldX + halfTileWidth) / halfTileWidth - worldY / halfTileHeight) / 2.f);
    *tileY = (int)((float)((worldX + halfTileWidth) / halfTileWidth + worldY / halfTileHeight) / 2.f);
}

bool ScenarioEditor::IsTileInSelectionPreview(int tileX, int tileY) const {
    int left = SDL_min(m_SelectionPreviewStart.X, m_SelectionPreviewEnd.X);
    int right = SDL_max(m_SelectionPreviewStart.X, m_SelectionPreviewEnd.X);
    int top = SDL_min(m_SelectionPreviewStart.Y, m_SelectionPreviewEnd.Y);
    int bottom = SDL_max(m_SelectionPreviewStart.Y, m_SelectionPreviewEnd.Y);
    return tileX >= left && tileX <= right && tileY >= top && tileY <= bottom;
}
