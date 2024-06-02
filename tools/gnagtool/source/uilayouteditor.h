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
    UILayoutEditor(GnagTool *gnagTool, SDL_Renderer *renderer, const std::string& uiLayoutPath);

    void Update(float deltaTime) override;
    void DoGUI() override;

    void RenderEditorToTexture(SDL_Texture *texture);

private:
    SDL_Texture *m_EditorTexture;
    SDL_Renderer *m_Renderer;
    TTF_Font *m_Font;

    std::string m_UILayoutPath;
    UILayout m_UILayout;
};


#endif //GNAGTOOL_UILAYOUTEDITOR_H
