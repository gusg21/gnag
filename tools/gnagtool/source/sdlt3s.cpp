//
// Created by Angus Goucher on 6/10/2024.
//

#include "sdlt3s.h"

#include <iostream>
#include <fstream>

#include "gnagoswrapper.h"
#include "SDL_image.h"
#include "SDL.h"

SDLT3S::SDLT3S(SDL_Renderer* renderer, const std::string &t3sPath) {
    std::string t3sBaseDir = GnagOSWrapper::GetDirectoryFromPath(GnagOSWrapper::LocalizePath(t3sPath));
    std::string absoluteBaseDir = GnagOSWrapper::MakePathAbsolute(t3sBaseDir);
    printf("Absolute base dir: %s\n", absoluteBaseDir.c_str());
    printf("(Raw base dir: %s)\n", t3sBaseDir.c_str());

    // Find sprites
    std::ifstream t3sStream(t3sPath);
    std::string line;
    while (std::getline(t3sStream, line)) {
        // Looks like this also works with relative paths. Fix if you want, I guess!
        std::string fullPath = GnagOSWrapper::CombinePaths(absoluteBaseDir, line);
        if (GnagOSWrapper::DoesFileExist(fullPath)) {
            printf("Found valid path: %s\n", fullPath.c_str());

            m_Sprites.push_back(IMG_LoadTexture(renderer, fullPath.c_str()));
        }
    }
}

SDL_Texture *SDLT3S::GetTextureByIndex(uint32_t textureIndex) {
    return m_Sprites[textureIndex];
}
uint32_t SDLT3S::GetCount() {
    return m_Sprites.size();
}
