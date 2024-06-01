//
// Created by Angus Goucher on 5/25/2024.
//

#ifndef GNAGTOOL_SCENARIOEDITOR_H
#define GNAGTOOL_SCENARIOEDITOR_H

#include "SDL.h"
#include "imgui.h"
#include "toolgui.h"
#include "scenario.h"

class GnagTool;

class TilePos {
public:
    int X, Y;

    friend bool operator==(const TilePos &lhs, const TilePos &rhs) {
        return lhs.X == rhs.X && lhs.Y == rhs.Y;
    }
};

class ScenarioEditor : public ToolGUI {
public:
    explicit ScenarioEditor(GnagTool *gnagTool, SDL_Renderer *renderer, const Scenario &scenario, const std::string& fileName);

    void DoGUI() override;

private:
    void RenderEditorToTexture(SDL_Texture **texture, ImVec2 editorSize);
    void DrawGrid();
    void GetWorldPosFromTilePos(int tileX, int tileY, float *worldX, float *worldY) const;
    void GetTilePosFromWorldPos(float worldX, float worldY, int *tileX, int *tileY) const;
    void GetScreenPosFromWorldPos(float worldX, float worldY, float* screenX, float* screenY) const;
    bool IsTileInSelectionPreview(int tileX, int tileY) const;
    void GetSelectionBounds(int* left, int* right, int* top, int* bottom) const;

    GnagTool *m_GnagTool;
    SDL_Renderer *m_Renderer;
    SDL_Texture *m_EditorTexture;
    ImVec2 m_LastFrameEditorSize{};
    Scenario m_Scenario;
    std::string m_FilePath{};

    float m_MousePosX = 0;
    float m_MousePosY = 0;
    float m_ViewX = 0;
    float m_ViewY = 0;
    float m_ViewWidth = 0;
    float m_ViewHeight = 0;
    float m_ViewPanSpeed = 300.f;
    float m_TileWidth = 64.f;
    float m_TileHeight = 32.f;
    std::vector<TilePos> m_Selection{};
    TilePos m_SelectionPreviewStart{};
    TilePos m_SelectionPreviewEnd{};
    bool m_SelectionPreviewVisible = false;
    int m_HazardType = 1;
    int m_ScenarioResizeSize[2] = {10, 10};
    bool m_WantOpenTileEditor = false;
    bool m_Focused = false;
    char m_SaveAsFilePath[256] {0};
};


#endif //GNAGTOOL_SCENARIOEDITOR_H
