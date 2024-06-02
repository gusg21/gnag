//
// Created by Angus Goucher on 5/31/2024.
//

#include "uilayout.h"


void UILayout::FromJSON(cJSON *json) {
    cJSON *buttonJson;
    cJSON_ArrayForEach(buttonJson, cJSON_GetObjectItem(json, "Buttons")) {
        Button button { };

        button.X = (int32_t) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(buttonJson, "Pos"), 0));
        button.Y = (int32_t) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(buttonJson, "Pos"), 1));

        button.Width = (int32_t) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(buttonJson, "Size"), 0));
        button.Height = (int32_t) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(buttonJson, "Size"), 1));

        button.SpriteIndex = (uint32_t) cJSON_GetNumberValue(cJSON_GetObjectItem(buttonJson, "Sprite Index"));
        button.Color = {
                (float) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(buttonJson, "Color"), 0)),
                (float) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(buttonJson, "Color"), 1)),
                (float) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(buttonJson, "Color"), 2)),
                (float) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(buttonJson, "Color"), 3))
        };
        button.CallbackID = (uint32_t) cJSON_GetNumberValue(cJSON_GetObjectItem(buttonJson, "Callback"));
        button.UpdaterID = (uint32_t) cJSON_GetNumberValue(cJSON_GetObjectItem(buttonJson, "Updater"));

        Buttons.push_back(button);
    }

    cJSON *imageJson;
    cJSON_ArrayForEach(imageJson, cJSON_GetObjectItem(json, "Images")) {
        Image image { };

        image.X = (int32_t) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(imageJson, "Pos"), 0));
        image.Y = (int32_t) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(imageJson, "Pos"), 1));

        image.Width = (int32_t) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(imageJson, "Size"), 0));
        image.Height = (int32_t) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(imageJson, "Size"), 1));

        image.SpriteIndex = (uint32_t) cJSON_GetNumberValue(cJSON_GetObjectItem(imageJson, "Sprite Index"));
        image.Color = {
                (float) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(imageJson, "Color"), 0)),
                (float) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(imageJson, "Color"), 1)),
                (float) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(imageJson, "Color"), 2)),
                (float) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(imageJson, "Color"), 3))
        };
        image.UpdaterID = (uint32_t) cJSON_GetNumberValue(cJSON_GetObjectItem(imageJson, "Updater"));
        image.RenderOnTop = cJSON_IsTrue(cJSON_GetObjectItem(imageJson, "Render On Top"));

        Images.push_back(image);
    }

    cJSON *fillBarJson;
    cJSON_ArrayForEach(fillBarJson, cJSON_GetObjectItem(json, "Fillbars")) {
        FillBar fillBar { };

        fillBar.X = (int32_t) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(fillBarJson, "Pos"), 0));
        fillBar.Y = (int32_t) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(fillBarJson, "Pos"), 1));

        fillBar.Width = (int32_t) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(fillBarJson, "Size"), 0));
        fillBar.Height = (int32_t) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(fillBarJson, "Size"), 1));

        fillBar.BackgroundColor = {
                (float) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(fillBarJson, "Background Color"), 0)),
                (float) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(fillBarJson, "Background Color"), 1)),
                (float) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(fillBarJson, "Background Color"), 2)),
                (float) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(fillBarJson, "Background Color"), 3))
        };
        fillBar.FillColor = {
                (float) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(fillBarJson, "Fill Color"), 0)),
                (float) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(fillBarJson, "Fill Color"), 1)),
                (float) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(fillBarJson, "Fill Color"), 2)),
                (float) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(fillBarJson, "Fill Color"), 3))
        };
        fillBar.UpdaterID = (uint32_t) cJSON_GetNumberValue(cJSON_GetObjectItem(fillBarJson, "Updater"));

        FillBars.push_back(fillBar);
    }

    cJSON *textJson;
    cJSON_ArrayForEach(textJson, cJSON_GetObjectItem(json, "Texts")) {
        Text text { };

        text.X = (int32_t) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(textJson, "Pos"), 0));
        text.Y = (int32_t) cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(textJson, "Pos"), 1));

        text.Centered = cJSON_IsTrue(cJSON_GetObjectItem(textJson, "Centered"));
        text.UpdaterID = (uint32_t) cJSON_GetNumberValue(cJSON_GetObjectItem(textJson, "Updater"));

        text.InitialText = std::string(cJSON_GetStringValue(cJSON_GetObjectItem(textJson, "Initial Text")));

        Texts.push_back(text);
    }
}

void UILayout::ToJSON(cJSON *json) {

}
