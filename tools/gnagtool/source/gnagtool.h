//
// Created by Angus Goucher on 5/25/2024.
//

#ifndef GNAGTOOL_GNAGTOOL_H
#define GNAGTOOL_GNAGTOOL_H

#include "fileexplorer.h"
#include "scenarioeditor.h"
#include "toolgui.h"
#include "SDL.h"

class GnagTool {
public:
    explicit GnagTool(SDL_Renderer* renderer);

    void Update(float deltaTime);
    void DoGUI();
    void AddGUI(ToolGUI* gui);
    void CloseGUI(ToolGUI* gui);

    float GetDeltaTime() const { return m_DeltaTime; }

    SDL_Renderer* GetRenderer() { return m_Renderer; }
    SDL_Texture* GetEmptyTileTexture() { return m_EmptyTileTexture; }
    SDL_Texture* GetSpikesTileTexture() { return m_SpikesTileTexture; }
    SDL_Texture* GetQuestionTileTexture() { return m_SpikesTileTexture; }

private:
    // Refs
    std::vector<ToolGUI*> m_GUIs {};
    SDL_Renderer* m_Renderer;

    // Assets
    SDL_Texture* m_EmptyTileTexture;
    SDL_Texture* m_SpikesTileTexture;
    SDL_Texture* m_QuestionTileTexture;

    // Internals
    float m_FPS = 0.f;
    float m_DeltaTime = 0.f;
    float m_MenuBarHeight = 0.f;
    std::vector<ToolGUI*> m_GUIsToClose{};
    void InternalGUI();
};


#endif //GNAGTOOL_GNAGTOOL_H
