//
// Created by Angus Goucher on 6/10/2024.
//

#ifndef GNAGTOOL_SDLT3S_H
#define GNAGTOOL_SDLT3S_H

#include <string>
#include <vector>

#include "SDL.h"

class SDLT3S {
public:
    explicit SDLT3S(SDL_Renderer* renderer, const std::string& t3sPath);

    SDL_Texture* GetTextureByIndex(uint32_t textureIndex);
    uint32_t GetCount();

private:
    std::vector<SDL_Texture*> m_Sprites{};
};

#endif //GNAGTOOL_SDLT3S_H
