#ifndef DEBUG_CONSOLE_H
#define DEBUG_CONSOLE_H

#include <3ds.h>
#include <citro2d.h>

#define DEBUG_CONSOLE_LINE_LENGTH 80
#define DEBUG_CONSOLE_LINE_COUNT 20

typedef struct {

    bool enabled;

    char lines[DEBUG_CONSOLE_LINE_LENGTH][DEBUG_CONSOLE_LINE_COUNT];
    C2D_TextBuf buffers[DEBUG_CONSOLE_LINE_COUNT];
    C2D_Text texts[DEBUG_CONSOLE_LINE_COUNT];

} debug_console_t;

void DebugConsole_Init();
void DebugConsole_Print(const char* text);
void DebugConsole_Draw();

#endif // DEBUG_CONSOLE_H