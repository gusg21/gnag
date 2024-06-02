//
// Created by Angus Goucher on 5/31/2024.
//

#include "uilayouteditor.h"
#include "imgui.h"
#include <cstdio>

UILayoutEditor::UILayoutEditor(GnagTool *gnagTool, SDL_Renderer *renderer, const std::string &uiLayoutPath) : ToolGUI(
        gnagTool) {
    m_Renderer = renderer;
    m_UILayoutPath = uiLayoutPath;
    m_UILayout.LoadFromJSON(m_UILayoutPath);

    m_EditorTexture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480);
    m_Font = TTF_OpenFont("assets/OpenSans-Regular.ttf", 12);
    if (m_Font == nullptr)
        printf("%s", TTF_GetError());
}

void UILayoutEditor::Update(float deltaTime) {
    ToolGUI::Update(deltaTime);
}

void UILayoutEditor::DoGUI() {

    ImGui::SetNextWindowSize({ 700, 520 }, ImGuiCond_FirstUseEver);
    ImGui::Begin("UI Layout Editor");
    {
        ImGui::Text("%zu buttons, %zu texts, %zu fillbars, %zu images", m_UILayout.Buttons.size(), m_UILayout.Texts.size(), m_UILayout.FillBars.size(), m_UILayout.Images.size());
        RenderEditorToTexture(m_EditorTexture);
        ImVec2 imagePos = ImGui::GetCursorPos();
        ImGui::Image(m_EditorTexture, { 640, 480 });

        for (uint32_t buttonIndex = 0; buttonIndex < m_UILayout.Buttons.size(); buttonIndex++) {
            SDL_Rect rect = {
                    m_UILayout.Buttons[buttonIndex].X,
                    m_UILayout.Buttons[buttonIndex].Y,
                    static_cast<int>(m_UILayout.Buttons[buttonIndex].Width),
                    static_cast<int>(m_UILayout.Buttons[buttonIndex].Height)
            };
            ImGui::PushID((int) buttonIndex);
            ImGui::SetCursorPos({ static_cast<float>(rect.x) + imagePos.x, static_cast<float>(rect.y) + imagePos.y });
            ImGui::InvisibleButton("Button", { static_cast<float>(rect.w), static_cast<float>(rect.h) });
            if (ImGui::BeginItemTooltip()) {
                ImGui::Text("sprite index %d", m_UILayout.Buttons[buttonIndex].SpriteIndex);
                ImGui::Text("updater id %d", m_UILayout.Buttons[buttonIndex].UpdaterID);
                ImGui::Text("callback id %d", m_UILayout.Buttons[buttonIndex].CallbackID);
                ImGui::EndTooltip();
            }
            ImGui::PopID();
        }
    }
    ImGui::End();
}

void UILayoutEditor::RenderEditorToTexture(SDL_Texture *texture) {
    SDL_SetRenderTarget(m_Renderer, texture);
    SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 0);
    SDL_RenderClear(m_Renderer);
    {
        for (uint32_t buttonIndex = 0; buttonIndex < m_UILayout.Buttons.size(); buttonIndex++) {
            SDL_Rect rect = {
                    m_UILayout.Buttons[buttonIndex].X,
                    m_UILayout.Buttons[buttonIndex].Y,
                    static_cast<int>(m_UILayout.Buttons[buttonIndex].Width),
                    static_cast<int>(m_UILayout.Buttons[buttonIndex].Height)
            };
            SDL_SetRenderDrawColor(m_Renderer, 0, 230, 0, 255);
            SDL_RenderDrawRect(m_Renderer, &rect);

            // Draw Text
            SDL_Surface *textSurf = TTF_RenderText_Solid(m_Font, "Hallo!", { 0, 230, 0, 255 });
            SDL_Rect textRect = {
                    rect.x + 2, rect.y + 2, textSurf->w, textSurf->h };
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(m_Renderer, textSurf);
            SDL_RenderCopy(m_Renderer, textTexture, nullptr, &textRect);
            SDL_DestroyTexture(textTexture);


        }
    }
    SDL_SetRenderTarget(m_Renderer, nullptr);
}
