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
    GnagTool(SDL_Renderer* renderer);

    void Update(float deltaTime);
    void DoGUI();

private:
    // Refs
    std::vector<ToolGUI*> m_GUIs {};

    // Internals
    float m_FPS = 0.f;
    float m_MenuBarHeight = 0.f;
    void InternalGUI();
};


#endif //GNAGTOOL_GNAGTOOL_H
