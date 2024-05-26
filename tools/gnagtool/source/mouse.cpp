//
// Created by Angus Goucher on 5/25/2024.
//

#include "mouse.h"

#include "SDL.h"

uint32_t Mouse::m_ThisFrame = 0;
uint32_t Mouse::m_LastFrame = 0;

bool Mouse::IsLeftPressed() {
    return (m_ThisFrame & SDL_BUTTON_LEFT) && !(m_LastFrame & SDL_BUTTON_LEFT);
}

bool Mouse::IsRightPressed() {
    return (m_ThisFrame & SDL_BUTTON_RIGHT) && !(m_LastFrame & SDL_BUTTON_RIGHT);
}

bool Mouse::IsLeftReleased() {
    return !(m_ThisFrame & SDL_BUTTON_LEFT) && (m_LastFrame & SDL_BUTTON_LEFT);
}

bool Mouse::IsRightReleased() {
    return !(m_ThisFrame & SDL_BUTTON_RIGHT) && (m_LastFrame & SDL_BUTTON_RIGHT);
}

bool Mouse::IsLeftDown() {
    return (m_ThisFrame & SDL_BUTTON_LEFT);
}

bool Mouse::IsRightDown() {
    return (m_ThisFrame & SDL_BUTTON_RIGHT);
}

void Mouse::Update() {
    m_LastFrame = m_ThisFrame;
    m_ThisFrame = SDL_GetMouseState(nullptr, nullptr);
}
