//
// Created by Angus Goucher on 5/25/2024.
//

#ifndef GNAGTOOL_MOUSE_H
#define GNAGTOOL_MOUSE_H


#include <cstdint>

class Mouse {
public:
    static bool IsLeftPressed();
    static bool IsRightPressed();
    static bool IsLeftReleased();
    static bool IsRightReleased();
    static bool IsLeftDown();
    static bool IsRightDown();

    static void Update();

private:
    static uint32_t m_ThisFrame;
    static uint32_t m_LastFrame;
};


#endif //GNAGTOOL_MOUSE_H
