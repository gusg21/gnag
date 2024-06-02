//
// Created by Angus Goucher on 5/31/2024.
//

#ifndef GNAGTOOL_UILAYOUT_H
#define GNAGTOOL_UILAYOUT_H

#include <string>
#include <vector>
#include "jsonserializable.h"

class ColorFloat {
public:
    float R, G, B, A;
};

class Button {
public:
    int32_t X, Y;
    uint32_t Width, Height;
    uint32_t SpriteIndex;
    ColorFloat Color;
    uint32_t CallbackID;
    uint32_t UpdaterID;
};

class Image {
public:
    int32_t X, Y;
    uint32_t Width, Height;
    uint32_t SpriteIndex;
    ColorFloat Color;
    bool RenderOnTop;
    uint32_t UpdaterID;
};

class FillBar {
public:
    int32_t X, Y;
    uint32_t Width, Height;
    ColorFloat BackgroundColor;
    ColorFloat FillColor;
    uint32_t UpdaterID;
};

class Text {
public:
    std::string InitialText;
    int32_t X, Y;
    bool Centered;
    uint32_t UpdaterID;
};

class UILayout : public JSONSerializable {
public:
    std::vector<Button> Buttons { };
    std::vector<Image> Images { };
    std::vector<FillBar> FillBars { };
    std::vector<Text> Texts { };

protected:
    void FromJSON(cJSON *json) override;
    void ToJSON(cJSON *json) override;
};

#endif // GNAGTOOL_UILAYOUT_H
