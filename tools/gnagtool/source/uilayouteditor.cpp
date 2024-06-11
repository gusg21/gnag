//
// Created by Angus Goucher on 5/31/2024.
//

#include "uilayouteditor.h"

#include <cstdio>

#include "imgui.h"

#include "gnagtool.h"
#include "uilayout.h"

UILayoutEditor::UILayoutEditor(GnagTool *gnagTool, SDL_Renderer *renderer, const std::string &uiLayoutPath) : ToolGUI(
        gnagTool) {
    m_Renderer = renderer;
    m_UILayoutPath = uiLayoutPath;
    UILayout_InitFromFile(&m_UILayout, m_UILayoutPath.c_str());

    m_EditorTexture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                        BOTTOM_SCREEN_WIDTH, BOTTOM_SCREEN_HEIGHT);
    m_Font = TTF_OpenFont("assets/OpenSans-Regular.ttf", 12);
    if (m_Font == nullptr)
        printf("%s", TTF_GetError());
}

void UILayoutEditor::Update(float deltaTime) {
    ToolGUI::Update(deltaTime);
}

void UILayoutEditor::DoGUI() {

    ImGui::SetNextWindowSize({ BOTTOM_SCREEN_WIDTH + 100, BOTTOM_SCREEN_HEIGHT + 100 }, ImGuiCond_FirstUseEver);
    ImGui::Begin("UI Layout Editor");
    {
        if (ImGui::Button("Save UI Layout")) {
            UILa
        }

        ImGui::Checkbox("Show Colors", &m_ShowColors);
        ImGui::SameLine();
        ImGui::Button("Simulate Button Pressed");
        m_SimulatePressed = ImGui::IsItemActive();
        if (m_SimulatePressed) m_ShowColors = true;
        ImGui::SameLine();
        ImGui::Checkbox("Super Zoom 3000", &m_UseSuperZoom3000);
        ImGui::SameLine();
        ImGui::TextDisabled("Text may be different size in build");

        ImGui::BeginTable("UI Layout Editor Table", 2, ImGuiTableFlags_Resizable);
        ImGui::TableNextColumn();
        ImGui::BeginChild("UI Layout Editor Objects");
        {
            ImGui::SeparatorText("Buttons");
            for (int buttonIndex = 0; buttonIndex < UILAYOUT_MAX_BUTTON_COUNT; buttonIndex++) {
                button_data_t *buttonData = &m_UILayout.button_datas[buttonIndex];
                if (buttonData->initialized) {
                    ImGui::PushID(buttonIndex);
                    if (m_HoveredButtonIndex == buttonIndex) {
                        ImGui::Text("*");
                        ImGui::SameLine();
                    }
                    if (ImGui::CollapsingHeader("Button")) {
                        ImGui::InputFloat2("Position", &buttonData->pos.x);
                        ImGui::InputFloat2("Size", &buttonData->size.x);
                        ImGui::InputInt("Sprite Index", (int *) &buttonData->sprite_idx);
                        ImGui::ColorEdit4("Color", &buttonData->color.r);
                        ImGui::InputInt("Updater ID", (int *) &buttonData->updater_type);
                        buttonData->updater_type = CLAMP(buttonData->updater_type,
                                                         static_cast<button_updater_type_e>(0),
                                                         BUTTON_UPDATER_COUNT);
                        ImGui::InputInt("Callback ID", (int *) &buttonData->callback_type);
                        buttonData->callback_type = CLAMP(buttonData->callback_type,
                                                          static_cast<button_callback_type_e>(0),
                                                          BUTTON_CALLBACK_COUNT);
                    }
                    ImGui::PopID();
                }
            }
            ImGui::SeparatorText("Images");
            for (int imageIndex = 0; imageIndex < UILAYOUT_MAX_IMAGE_COUNT; imageIndex++) {
                image_data_t *imageData = &m_UILayout.image_datas[imageIndex];
                if (imageData->initialized) {
                    ImGui::PushID(imageIndex);
                    if (m_HoveredImageIndex == imageIndex) {
                        ImGui::Text("*");
                        ImGui::SameLine();
                    }
                    if (ImGui::CollapsingHeader("Image")) {
                        ImGui::InputFloat2("Position", &imageData->pos.x);
                        ImGui::InputFloat2("Size", &imageData->size.x);
                        ImGui::InputInt("Sprite", (int *) &imageData->sprite_idx);
                        imageData->sprite_idx = CLAMP(imageData->sprite_idx, 0,
                                                      m_GnagTool->GetUISprites()->GetCount() - 1);
                        ImGui::InputInt("Updater ID", (int *) &imageData->updater_type);
                        imageData->updater_type = CLAMP(imageData->updater_type,
                                                        static_cast<image_updater_type_e>(0),
                                                        IMAGE_UPDATER_COUNT);
                        ImGui::ColorEdit4("Color", &imageData->color.r);
                        ImGui::Checkbox("Render On Top", &imageData->render_on_top);
                        ImGui::TextDisabled("Render On Top doesn't affect editor preview");
                    }
                    ImGui::PopID();
                }
            }
            ImGui::SeparatorText("Texts");
            for (int textIndex = 0; textIndex < UILAYOUT_MAX_BUTTON_COUNT; textIndex++) {
                text_data_t *textData = &m_UILayout.text_datas[textIndex];
                if (textData->initialized) {
                    ImGui::PushID(textIndex);
                    if (m_HoveredTextIndex == textIndex) {
                        ImGui::Text("*");
                        ImGui::SameLine();
                    }
                    if (ImGui::CollapsingHeader("Text")) {
                        ImGui::InputFloat2("Position", &textData->pos.x);
                        ImGui::Checkbox("Centered", &textData->centered);
                        ImGui::InputText("Initial Text", textData->initial_text, TEXT_DATA_MAX_TEXT_LENGTH);
                        ImGui::InputInt("Updater ID", (int *) &textData->updater_type);
                        textData->updater_type = CLAMP(textData->updater_type,
                                                       static_cast<text_updater_type_e>(0),
                                                       TEXT_UPDATER_COUNT);
                    }
                    ImGui::PopID();
                }
            }
            ImGui::SeparatorText("Fill Bars");
            for (int fillBarIndex = 0; fillBarIndex < UILAYOUT_MAX_FILL_BAR_COUNT; fillBarIndex++) {
                fill_bar_data_t *fillBarData = &m_UILayout.fill_bar_datas[fillBarIndex];
                if (fillBarData->initialized) {
                    ImGui::PushID(fillBarIndex);
                    if (m_HoveredFillBarIndex == fillBarIndex) {
                        ImGui::Text("*");
                        ImGui::SameLine();
                    }
                    if (ImGui::CollapsingHeader("Fill Bar")) {

                    }
                    ImGui::PopID();
                }
            }

        }
        ImGui::EndChild();
        ImGui::TableNextColumn();
        ImGui::BeginChild("UI Layout Editor Viewport", { 0.f, 0.f }, 0, ImGuiWindowFlags_HorizontalScrollbar);
        {
            RenderEditorToTexture(m_EditorTexture);
            ImVec2 imagePos = ImGui::GetCursorPos();
            ImVec2 imageSize = { BOTTOM_SCREEN_WIDTH, BOTTOM_SCREEN_HEIGHT };
            if (m_UseSuperZoom3000) {
                imageSize = { BOTTOM_SCREEN_WIDTH * 4, BOTTOM_SCREEN_HEIGHT * 4 };
            }
            ImGui::Image(m_EditorTexture, imageSize);

            bool anythingHovered = false;
            for (uint32_t buttonIndex = 0; buttonIndex < UILAYOUT_MAX_BUTTON_COUNT; buttonIndex++) {
                if (m_UILayout.button_datas[buttonIndex].initialized) {
                    SDL_Rect rect = {
                            static_cast<int>(m_UILayout.button_datas[buttonIndex].pos.x),
                            static_cast<int>(m_UILayout.button_datas[buttonIndex].pos.y),
                            static_cast<int>(m_UILayout.button_datas[buttonIndex].size.x),
                            static_cast<int>(m_UILayout.button_datas[buttonIndex].size.y)
                    };
                    ImGui::PushID((int) buttonIndex);
                    ImGui::SetCursorPos(
                            { static_cast<float>(rect.x) + imagePos.x, static_cast<float>(rect.y) + imagePos.y });
                    ImGui::InvisibleButton("Button", { static_cast<float>(rect.w), static_cast<float>(rect.h) });
                    if (ImGui::IsItemHovered()) {
                        m_HoveredButtonIndex = buttonIndex;
                        anythingHovered = true;
                    }
                    if (ImGui::BeginItemTooltip()) {
                        ImGui::Text("sprite index %d", m_UILayout.button_datas[buttonIndex].sprite_idx);
                        ImGui::Text("updater id %d", m_UILayout.button_datas[buttonIndex].updater_type);
                        ImGui::Text("callback id %d", m_UILayout.button_datas[buttonIndex].callback_type);
                        ImGui::EndTooltip();
                    }

                    ImGui::PopID();
                }
            }

            if (!anythingHovered) {
                m_HoveredButtonIndex = UINT32_MAX;
            }
        }
        ImGui::EndChild();
        ImGui::EndTable();
    }
    ImGui::End();
}

void UILayoutEditor::RenderEditorToTexture(SDL_Texture *texture) {
    SDL_SetRenderTarget(m_Renderer, texture);
    if (m_ShowColors) {
        SDL_SetRenderDrawColor(m_Renderer, CLEAR_COLOR_INTS);
    } else {
        SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 0);
    }
    SDL_RenderClear(m_Renderer);
    {
        for (uint32_t imageIndex = 0; imageIndex < UILAYOUT_MAX_IMAGE_COUNT; imageIndex++) {
            image_data_t *imageData = &m_UILayout.image_datas[imageIndex];
            if (imageData->initialized) {
                SDL_Rect imageRect = {
                        static_cast<int>(imageData->pos.x), static_cast<int>(imageData->pos.y),
                        static_cast<int>(imageData->size.x), static_cast<int>(imageData->size.y)
                };
                if (m_ShowColors) {
                    SDL_Texture *imageTexture = m_GnagTool->GetUISprites()->GetTextureByIndex(imageData->sprite_idx);
                    SDL_SetTextureColorMod(imageTexture, static_cast<uint8_t>(imageData->color.r * 255.f),
                                           static_cast<uint8_t>(imageData->color.g * 255.f),
                                           static_cast<uint8_t>(imageData->color.b * 255.f));
                    SDL_RenderCopy(m_Renderer, imageTexture, nullptr, &imageRect);
                    SDL_SetTextureColorMod(imageTexture, 255, 255, 255);
                } else {
                    SDL_SetRenderDrawColor(m_Renderer, 0, 230, 0, 255);
                    SDL_RenderDrawRect(m_Renderer, &imageRect);
                }
            }
        }

        for (uint32_t fillBarIndex = 0; fillBarIndex < UILAYOUT_MAX_FILL_BAR_COUNT; fillBarIndex++) {
            fill_bar_data_t *fillBarData = &m_UILayout.fill_bar_datas[fillBarIndex];
            if (fillBarData->initialized) {
                SDL_Rect bgRect = {
                        static_cast<int>(fillBarData->pos.x), static_cast<int>(fillBarData->pos.y),
                        static_cast<int>(fillBarData->size.x), static_cast<int>(fillBarData->size.y)
                };
                SDL_Rect fillRect = {
                        static_cast<int>(fillBarData->pos.x), static_cast<int>(fillBarData->pos.y),
                        static_cast<int>(fillBarData->size.x / 2.f), static_cast<int>(fillBarData->size.y)
                };
                if (m_ShowColors) {
                    SDL_SetRenderDrawColor(m_Renderer, static_cast<int>(fillBarData->bg_color.r * 255.f),
                                           static_cast<int>(fillBarData->bg_color.g * 255.f),
                                           static_cast<int>(fillBarData->bg_color.b * 255.f),
                                           static_cast<int>(fillBarData->bg_color.a * 255.f));
                    SDL_RenderFillRect(m_Renderer, &bgRect);
                    SDL_SetRenderDrawColor(m_Renderer, static_cast<int>(fillBarData->fill_color.r * 255.f),
                                           static_cast<int>(fillBarData->fill_color.g * 255.f),
                                           static_cast<int>(fillBarData->fill_color.b * 255.f),
                                           static_cast<int>(fillBarData->fill_color.a * 255.f));
                    SDL_RenderFillRect(m_Renderer, &fillRect);
                }
                else {
                    SDL_SetRenderDrawColor(m_Renderer, 0, 230, 0, 255);
                    SDL_RenderDrawRect(m_Renderer, &bgRect);
                    SDL_RenderFillRect(m_Renderer, &fillRect);
                }
            }
        }

        for (uint32_t buttonIndex = 0; buttonIndex < UILAYOUT_MAX_BUTTON_COUNT; buttonIndex++) {
            button_data_t *buttonData = &m_UILayout.button_datas[buttonIndex];
            if (buttonData->initialized) {
                SDL_Rect rect = {
                        static_cast<int>(buttonData->pos.x),
                        static_cast<int>(buttonData->pos.y),
                        static_cast<int>(buttonData->size.x),
                        static_cast<int>(buttonData->size.y)
                };
                SDL_SetRenderDrawColor(m_Renderer, 0, 230, 0, 255);
                SDL_RenderDrawRect(m_Renderer, &rect);
                if (m_ShowColors) {
                    colorf_t buttonColor = buttonData->color;
                    if (m_SimulatePressed) {
                        buttonColor = ButtonData_GetPressedColor(buttonColor);
                    }

                    SDL_SetRenderDrawColor(m_Renderer,
                                           (uint8_t) (buttonColor.r * 255.f),
                                           (uint8_t) (buttonColor.g * 255.f),
                                           (uint8_t) (buttonColor.b * 255.f),
                                           (uint8_t) (buttonColor.a * 255.f));

                    SDL_RenderFillRect(m_Renderer, &rect);
                }
            }
        }

        for (uint32_t textIndex = 0; textIndex < UILAYOUT_MAX_TEXT_COUNT; textIndex++) {
            text_data_t *textData = &m_UILayout.text_datas[textIndex];
            if (textData->initialized) {
                SDL_Color textColor;
                if (m_ShowColors) {
                    textColor = { 20, 20, 20, 255 };
                } else {
                    textColor = { 0, 230, 0, 255 };
                }

                // Draw Text
                SDL_Surface *textSurf = TTF_RenderText_Solid(m_Font, textData->initial_text, textColor);
                SDL_Rect textRect;
                if (textData->centered) {
                    textRect = {
                            static_cast<int>(textData->pos.x) - textSurf->w / 2,
                            static_cast<int>(textData->pos.y),
                            textSurf->w,
                            textSurf->h
                    };
                } else {
                    textRect = {
                            static_cast<int>(textData->pos.x),
                            static_cast<int>(textData->pos.y),
                            textSurf->w,
                            textSurf->h
                    };
                }

                SDL_Texture *textTexture = SDL_CreateTextureFromSurface(m_Renderer, textSurf);
                SDL_RenderCopy(m_Renderer, textTexture, nullptr, &textRect);
                SDL_DestroyTexture(textTexture);
            }
        }
    }
    SDL_SetRenderTarget(m_Renderer, nullptr);
}
