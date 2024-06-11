//
// Created by Angus Goucher on 5/25/2024.
//

#ifndef GNAGTOOL_GNAGTOOL_H
#define GNAGTOOL_GNAGTOOL_H

#include "SDL.h"
#include "fileexplorer.h"
#include "scenarioeditor.h"
#include "toolgui.h"
#include "sdlt3s.h"

class GnagTool {
public:
    explicit GnagTool(SDL_Renderer *renderer);

    void Update(float deltaTime);
    void DoGUI();
    void AddGUI(ToolGUI *gui);
    void CloseGUI(ToolGUI *gui);
    float GetDeltaTime() const { return m_DeltaTime; }

    SDL_Renderer *GetRenderer() { return m_Renderer; }
    SDLT3S *GetSprites() { return m_Sprites; }
    SDLT3S *GetUISprites() { return m_UISprites; }

private:
    // Refs
    std::vector<ToolGUI *> m_GUIs { };
    SDL_Renderer *m_Renderer;

    // Internals
    float m_FPS = 0.f;
    float m_DeltaTime = 0.f;
    float m_MenuBarHeight = 0.f;
    std::vector<ToolGUI *> m_GUIsToClose { };
    bool m_RebuildBuildinator = false;
    bool m_ShowImGuiDemo = false;
    SDLT3S *m_Sprites;
    SDLT3S *m_UISprites;
    void InternalGUI();
};


#endif //GNAGTOOL_GNAGTOOL_H
