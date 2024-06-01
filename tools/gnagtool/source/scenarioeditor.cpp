//
// Created by Angus Goucher on 5/25/2024.
//

#include <vector>

#include "scenarioeditor.h"
#include "imgui.h"
#include "gnagtool.h"

ScenarioEditor::ScenarioEditor(GnagTool *gnagTool, SDL_Renderer *renderer, const Scenario &scenario,
                               const std::string &fileName)
        : ToolGUI(gnagTool), m_Scenario(scenario) {
    m_GnagTool = gnagTool;
    m_Renderer = renderer;
    m_FilePath = fileName;

    m_EditorTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480);

    GetWorldPosFromTilePos(scenario.GridWidth / 2, scenario.GridHeight / 2, &m_ViewX, &m_ViewY);
}

void ScenarioEditor::DoGUI() {
    bool open = true;
    ImGui::SetNextWindowSize({500, 500}, ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Scenario Editor", &open)) {
        // Info
        ImGui::Text("Editing file %s", m_FilePath.c_str());
        if (ImGui::BeginItemTooltip()) {
            ImGui::Text("Grid Size %d x %d", m_Scenario.GridWidth, m_Scenario.GridHeight);
            ImGui::Text("View Pos %f x %f", m_ViewX, m_ViewY);
            ImGui::Text("%zu tiles selected", m_Selection.size());
            ImGui::EndTooltip();
        }

        // Buttons
        if (ImGui::Button("Resize Scenario...")) {
            ImGui::OpenPopup("ResizeScenario");
        }
        ImGui::SameLine();
        if (ImGui::Button("Save Scenario")) {
            m_Scenario.SaveToJSON(m_FilePath);
        }
        ImGui::SameLine();
        if (ImGui::Button("Save Scenario As...")) {
            std::string directory = GnagOSWrapper::GetDirectoryFromPath(m_FilePath);
            memcpy(m_SaveAsFilePath, directory.c_str(), 256);
            ImGui::OpenPopup("SaveScenarioAs");
        }

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 200);
        ImGui::BeginChild("Character List");
        {
            if (ImGui::Button("+ Character")) {
                m_Scenario.Characters.push_back(CharacterData{
                        0, 0, CharacterType::GOOD
                });
            }

            for (uint32_t characterIndex = 0; characterIndex < m_Scenario.Characters.size(); characterIndex++) {
                CharacterData *character = &m_Scenario.Characters[characterIndex];
                ImGui::PushID((int)characterIndex);
                if (ImGui::CollapsingHeader("Character")) {
                    ImGui::InputInt2("Tile Pos", &character->TileX);
                    ImGui::InputInt("Type", reinterpret_cast<int *>(&character->Type));
                    character->Type = static_cast<CharacterType>(
                            SDL_clamp(static_cast<int>(character->Type), 0, static_cast<int>(CharacterType::COUNT))
                    );
                    ImGui::Checkbox("Player Controlled", &character->IsPlayerControlled);
                }
                ImGui::PopID();
            }
        }
        ImGui::EndChild();
        ImGui::NextColumn();
        ImGui::BeginChild("Tile Grid Editor");
        {
            // Mouse
            ImVec2 availSize = ImGui::GetContentRegionAvail();
            ImVec2 screenPos = ImGui::GetCursorScreenPos();
            ImVec2 mousePos = ImGui::GetMousePos();
            m_MousePosX = mousePos.x - screenPos.x;
            m_MousePosY = mousePos.y - screenPos.y;

            int tileX, tileY;
            GetTilePosFromWorldPos(m_MousePosX + m_ViewX - (m_ViewWidth / 2.f),
                                   m_MousePosY + m_ViewY - (m_ViewHeight / 2.f), &tileX, &tileY);

            if (!ImGui::IsPopupOpen("TileEditor")) {
                bool inRange = m_Scenario.IsTileInRange(tileX, tileY);
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && inRange) {
                    // Selection
                    m_SelectionPreviewStart = {tileX, tileY};
                    m_SelectionPreviewEnd = {tileX, tileY};
                    m_SelectionPreviewVisible = true;
                } else if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && inRange) {
                    m_SelectionPreviewEnd = {tileX, tileY};
                    m_SelectionPreviewVisible = true;
                } else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && inRange && m_SelectionPreviewVisible) {
                    m_SelectionPreviewVisible = false;

                    m_Selection.clear();
                    int left, right, top, bottom;
                    GetSelectionBounds(&left, &right, &top, &bottom);
                    for (int xx = left; xx <= right; ++xx) {
                        for (int yy = top; yy <= bottom; ++yy) {
                            m_Selection.push_back({xx, yy});
                        }
                    }

                    if (ImGui::IsWindowFocused() && !m_Selection.empty()) {
                        ImGui::OpenPopup("TileEditor");
                    }
                }

                if (!inRange &&
                    (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseReleased(ImGuiMouseButton_Right))) {
                    m_Selection.clear();
                }
            }

            if (ImGui::IsWindowFocused()) {
                float deltaTime = m_GnagTool->GetDeltaTime();
                if (ImGui::IsKeyDown(ImGuiKey_A)) {
                    m_ViewX -= m_ViewPanSpeed * deltaTime;
                }
                if (ImGui::IsKeyDown(ImGuiKey_D)) {
                    m_ViewX += m_ViewPanSpeed * deltaTime;
                }
                if (ImGui::IsKeyDown(ImGuiKey_W)) {
                    m_ViewY -= m_ViewPanSpeed * deltaTime;
                }
                if (ImGui::IsKeyDown(ImGuiKey_S)) {
                    m_ViewY += m_ViewPanSpeed * deltaTime;
                }
            }

            // Render
            RenderEditorToTexture(&m_EditorTexture, availSize);
            ImGui::Image(m_EditorTexture, availSize, ImVec2{0, 0}, ImVec2{1, 1});

            // Tile Editor
            if (ImGui::BeginPopup("TileEditor")) {
                ImGui::InputInt("Hazard Type", &m_HazardType);
                m_HazardType = SDL_clamp(m_HazardType, 0, (int) HazardType::COUNT - 1);

                int left, right, top, bottom;
                GetSelectionBounds(&left, &right, &top, &bottom);
                for (int xx = left; xx <= right; ++xx) {
                    for (int yy = top; yy <= bottom; ++yy) {
                        HazardData data = m_Scenario.GetHazardDataAtTile(xx, yy);
                        data.HazardType = static_cast<HazardType>(m_HazardType);
                        m_Scenario.SetHazardDataAtTile(xx, yy, data);
                    }
                }

                ImGui::Text("Selection %d, %d, %d, %d", left, right, top, bottom);
                ImGui::Text("Start %d, %d - End %d, %d", m_SelectionPreviewStart.X, m_SelectionPreviewStart.Y,
                            m_SelectionPreviewEnd.X, m_SelectionPreviewEnd.Y);

                ImGui::EndPopup();
            }
        }
        ImGui::EndChild();

        // Resize Scenario
        if (ImGui::BeginPopup("ResizeScenario")) {
            ImGui::InputInt2("Size", m_ScenarioResizeSize);
            m_ScenarioResizeSize[0] = SDL_clamp(m_ScenarioResizeSize[0], 0, 100);
            m_ScenarioResizeSize[1] = SDL_clamp(m_ScenarioResizeSize[1], 0, 100);
            if (ImGui::Button("Resize!")) {
                m_Scenario.Resize(m_ScenarioResizeSize[0], m_ScenarioResizeSize[1]);
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        // Save Scenario As
        if (ImGui::BeginPopup("SaveScenarioAs")) {
            bool wantsSave = false;
            wantsSave = wantsSave ||
                        ImGui::InputText("Save As", m_SaveAsFilePath, 256, ImGuiInputTextFlags_EnterReturnsTrue);
            wantsSave = wantsSave || ImGui::Button("Save");

            if (wantsSave) {
                m_Scenario.SaveToJSON(m_SaveAsFilePath);
                m_FilePath = m_SaveAsFilePath;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        m_Focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
    }
    ImGui::End();

    if (!open) {
        m_GnagTool->CloseGUI(this);
    }

}

void ScenarioEditor::RenderEditorToTexture(SDL_Texture **texture, ImVec2 editorSize) {
    if ((int) m_LastFrameEditorSize.x != (int) editorSize.x || (int) m_LastFrameEditorSize.y != (int) editorSize.y) {
        SDL_DestroyTexture(*texture);
        *texture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int) editorSize.x,
                                     (int) editorSize.y);
    }

    SDL_SetRenderTarget(m_Renderer, *texture);
    SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 0);
    SDL_RenderClear(m_Renderer);
    {
        DrawGrid();

        for (uint32_t characterIndex = 0; characterIndex < m_Scenario.Characters.size(); characterIndex++) {
            CharacterData *character = &m_Scenario.Characters[characterIndex];
            float worldX, worldY;
            GetWorldPosFromTilePos(character->TileX, character->TileY, &worldX, &worldY);
            float screenX, screenY;
            GetScreenPosFromWorldPos(worldX, worldY, &screenX, &screenY);
            SDL_Rect dst = {
                    static_cast<int>(screenX - 32), static_cast<int>(screenY - 86), 64, 96
            };
            SDL_RenderCopy(m_Renderer, m_GnagTool->GetCharacterTexture(character->Type), nullptr, &dst);
        }
    }
    SDL_RenderPresent(m_Renderer);

    SDL_SetRenderTarget(m_Renderer, nullptr);

    m_LastFrameEditorSize = editorSize;

    m_ViewWidth = editorSize.x;
    m_ViewHeight = editorSize.y;
}

void ScenarioEditor::DrawGrid() {
    for (int xx = m_Scenario.GridWidth - 1; xx >= 0; xx--) {
        for (int yy = 0; yy < m_Scenario.GridHeight; yy++) {
            float worldX, worldY;
            GetWorldPosFromTilePos(xx, yy, &worldX, &worldY);
            float screenX, screenY;
            GetScreenPosFromWorldPos(worldX, worldY, &screenX, &screenY);

            // Render sprites
            SDL_Texture *tileTex;
            HazardData hazard = m_Scenario.GetHazardDataAtTile(xx, yy);
            if (hazard.HazardType == HazardType::NONE) {
                tileTex = m_GnagTool->GetEmptyTileTexture();
            } else if (hazard.HazardType == HazardType::SPIKES) {
                tileTex = m_GnagTool->GetSpikesTileTexture();
            } else {
                tileTex = m_GnagTool->GetQuestionTileTexture();
            }
            SDL_Rect dest = {
                    static_cast<int>(screenX - 32), static_cast<int>(screenY - 48), 64, 64
            };
            SDL_RenderCopy(m_Renderer, tileTex, nullptr, &dest);

            // Render grid
            if (IsTileInSelectionPreview(xx, yy) && m_SelectionPreviewVisible) {
                SDL_SetRenderDrawColor(m_Renderer, 240, 150, 130, 255);
            } else if (std::find(m_Selection.begin(), m_Selection.end(), TilePos{xx, yy}) != m_Selection.end()) {
                SDL_SetRenderDrawColor(m_Renderer, 240, 230, 100, 255);
            } else {
                SDL_SetRenderDrawColor(m_Renderer, 230, 20, 10, 255);
            }

            SDL_Point gridTilePoints[5] = {
                    {static_cast<int>(screenX - m_TileWidth / 2), static_cast<int>(screenY)},
                    {static_cast<int>(screenX),                   static_cast<int>(screenY - m_TileHeight / 2)},
                    {static_cast<int>(screenX + m_TileWidth / 2), static_cast<int>(screenY)},
                    {static_cast<int>(screenX),                   static_cast<int>(screenY + m_TileHeight / 2)},
                    {static_cast<int>(screenX - m_TileWidth / 2), static_cast<int>(screenY)}
            };
            SDL_RenderDrawLines(m_Renderer, gridTilePoints, 5);
        }
    }
}

void ScenarioEditor::GetWorldPosFromTilePos(int tileX, int tileY, float *worldX, float *worldY) const {
    float halfTileWidth = (float) m_TileWidth / 2.f;
    float halfTileHeight = (float) m_TileHeight / 2.f;
    *worldX = (float) (tileY + tileX) * halfTileWidth;
    *worldY = (float) (tileY - tileX) * halfTileHeight;
}

void ScenarioEditor::GetTilePosFromWorldPos(float worldX, float worldY, int *tileX, int *tileY) const {
    float halfTileWidth = (float) m_TileWidth / 2.f;
    float halfTileHeight = (float) m_TileHeight / 2.f;
    *tileX = (int) ((float) ((worldX + halfTileWidth) / halfTileWidth - worldY / halfTileHeight) / 2.f);
    *tileY = (int) ((float) ((worldX + halfTileWidth) / halfTileWidth + worldY / halfTileHeight) / 2.f);
}

bool ScenarioEditor::IsTileInSelectionPreview(int tileX, int tileY) const {
    int left, right, top, bottom;
    GetSelectionBounds(&left, &right, &top, &bottom);
    return tileX >= left && tileX <= right && tileY >= top && tileY <= bottom;
}

void ScenarioEditor::GetSelectionBounds(int *left, int *right, int *top, int *bottom) const {
    *left = SDL_min(m_SelectionPreviewStart.X, m_SelectionPreviewEnd.X);
    *right = SDL_max(m_SelectionPreviewStart.X, m_SelectionPreviewEnd.X);
    *top = SDL_min(m_SelectionPreviewStart.Y, m_SelectionPreviewEnd.Y);
    *bottom = SDL_max(m_SelectionPreviewStart.Y, m_SelectionPreviewEnd.Y);
}

void ScenarioEditor::GetScreenPosFromWorldPos(float worldX, float worldY, float *screenX, float *screenY) const {
    *screenX = worldX + -m_ViewX + m_ViewWidth / 2.f;
    *screenY = worldY + -m_ViewY + m_ViewHeight / 2.f;
}
