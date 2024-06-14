//
// Created by Angus Goucher on 5/31/2024.
//

#ifndef GNAGTOOL_UILAYOUTEDITOR_H
#define GNAGTOOL_UILAYOUTEDITOR_H

#include <string>
#include "SDL.h"
#include "SDL_ttf.h"

#include "toolgui.h"
#include "uilayout.h"

class UILayoutEditor : public ToolGUI {
public:
    UILayoutEditor(GnagTool *gnagTool, int guiID, SDL_Renderer *renderer, const std::string &uiLayoutPath);

    void Update(float deltaTime) override;
    void DoGUI() override;

    void RenderEditorToTexture(SDL_Texture *texture);

private:
    SDL_Texture *m_EditorTexture;
    SDL_Renderer *m_Renderer;
    TTF_Font *m_Font;

    std::string m_UILayoutPath;
    ui_layout_t m_UILayout;

    std::string m_SaveAsPath;

    uint32_t m_HoveredButtonIndex = UINT32_MAX;
    uint32_t m_HoveredTextIndex = UINT32_MAX;
    uint32_t m_HoveredImageIndex = UINT32_MAX;
    uint32_t m_HoveredFillBarIndex = UINT32_MAX;
    bool m_ShowColors = true;
    bool m_SimulatePressed = false;
    bool m_UseSuperZoom3000 = false;
};


#endif //GNAGTOOL_UILAYOUTEDITOR_H
