#include "debugconsole.h"

#include <citro2d.h>

debug_console_t console;
C2D_Font font;

void DebugConsole_Init() {
    console.enabled = true;

    memset(console.lines, 0, sizeof(char) * DEBUG_CONSOLE_LINE_COUNT * DEBUG_CONSOLE_LINE_LENGTH);
    memset(console.texts, 0, sizeof(C2D_Text) * DEBUG_CONSOLE_LINE_COUNT);

    for (u32 buffer_index = 0; buffer_index < DEBUG_CONSOLE_LINE_COUNT; buffer_index++) {
        console.buffers[buffer_index] = C2D_TextBufNew(DEBUG_CONSOLE_LINE_LENGTH * 2);
    }

    font = C2D_FontLoadSystem(CFG_REGION_USA);
}

void DebugConsole_Print(const char* text, size_t text_len) {
    for (s32 line_index = DEBUG_CONSOLE_LINE_COUNT - 1; line_index > 0; line_index--) {
        size_t next_line_len = strlen(console.lines[line_index - 1]);
        memcpy(console.lines[line_index], console.lines[line_index - 1], next_line_len);
    } 

    memcpy(console.lines[0], text, text_len);    

    for (u32 text_index = 0; text_index < DEBUG_CONSOLE_LINE_COUNT; text_index++) {
        C2D_TextBufClear(console.buffers[text_index]);
        C2D_TextParse(&console.texts[text_index], console.buffers[text_index], console.lines[text_index]);
        C2D_TextOptimize(&console.texts[text_index]);
    }
}

void DebugConsole_Draw() {
    if (console.enabled) {
        float line_height = 10.f;

        for (u32 line_index = 0; line_index < DEBUG_CONSOLE_LINE_COUNT; line_index++) {
            C2D_DrawRectSolid(0.f, (float)line_index * line_height, 0.f, console.texts[line_index].width * 0.5f, line_height, C2D_Color32(255, 255, 255, 255));
            C2D_DrawText(&console.texts[line_index], 0, 0.f, (float)line_index * line_height, 0.f, 0.5f, 0.5f);
        }
    }
}

void DebugConsole_ToggleEnabled() {
    console.enabled = !console.enabled;
}
