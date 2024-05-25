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

class ScenarioEditor : public ToolGUI {
public:
    explicit ScenarioEditor(GnagTool *gnagTool, SDL_Renderer *renderer, Scenario scenario);

    void Update(float deltaTime);
    void DoGUI() override;

private:
    void RenderEditorToTexture(SDL_Texture** texture, ImVec2 editorSize);
    void DrawGrid();
    void GetWorldPosFromTilePos(int tileX, int tileY, int* worldX, int* worldY) const;

    GnagTool *m_GnagTool;
    SDL_Renderer *m_Renderer;
    SDL_Texture *m_EditorTexture;
    ImVec2 m_LastFrameEditorSize{};
    bool m_Focused = false;
    Scenario m_Scenario;

    float m_ViewX = 0;
    float m_ViewY = 0;
    float m_ViewWidth = 0;
    float m_ViewHeight = 0;
    float m_ViewPanSpeed = 300.f;
    int m_TileWidth = 64.f;
    int m_TileHeight = 32.f;
};


#endif //GNAGTOOL_SCENARIOEDITOR_H
