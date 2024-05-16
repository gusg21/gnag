#ifndef DEBUG_CONSOLE_H
#define DEBUG_CONSOLE_H

#include <3ds.h>
#include <citro2d.h>

#define DEBUG_CONSOLE_LINE_LENGTH 40
#define DEBUG_CONSOLE_LINE_COUNT 20

typedef struct {

    bool enabled;
    gfxScreen_t screen;

    char texts[DEBUG_CONSOLE_LINE_LENGTH][DEBUG_CONSOLE_LINE_COUNT];

} debug_console_t;

void DebugConsole_Init(gfxScreen_t screen);
void DebugConsole_Print(const char* text);
void DebugConsole_Draw();

#endif // DEBUG_CONSOLE_H