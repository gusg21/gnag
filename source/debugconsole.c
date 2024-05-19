#include "debugconsole.h"

#include <citro2d.h>

#include "mathf.h"

debug_console_t console;
C2D_Font font;

void DebugConsole_Init() {
    console.enabled = true;

    memset(console.lines, 0, sizeof(char) * DEBUG_CONSOLE_LINE_COUNT * DEBUG_CONSOLE_LINE_LENGTH);
    memset(console.texts, 0, sizeof(C2D_Text) * DEBUG_CONSOLE_LINE_COUNT);

    console.buffer = C2D_TextBufNew(DEBUG_CONSOLE_LINE_LENGTH * DEBUG_CONSOLE_LINE_COUNT * 20);

    font = C2D_FontLoadSystem(CFG_REGION_USA);
}

void DebugConsole_Update(float delta_secs) {
    console.time_since += delta_secs;
}

void DebugConsole_Print(const char* text, size_t text_len) {
    // for (s32 line_index = DEBUG_CONSOLE_LINE_COUNT - 1; line_index > 0; line_index--) {
    //     size_t next_line_len = strlen(console.lines[line_index - 1]);
    //     memcpy(console.lines[line_index], console.lines[line_index - 1], next_line_len);
    // } 

    console.time_since = 0.f;

    memcpy(console.lines[0], text, text_len);

    svcOutputDebugString(text, text_len);

    C2D_TextBufClear(console.buffer);
    C2D_TextParse(&console.texts[0], console.buffer, console.lines[0]);
    C2D_TextOptimize(&console.texts[0]);
}

void DebugConsole_PrintInt(s32 i) {
    char buffer[DEBUG_CONSOLE_LINE_LENGTH] = {0};
    int length = snprintf(buffer, DEBUG_CONSOLE_LINE_LENGTH - 1, "%ld", i);
    DebugConsole_Print(buffer, length + 1);
}

void DebugConsole_Draw() {
    if (console.enabled) {
        float line_height = 10.f;
        float x = Mathf_Max(0.f, 20.f - console.time_since * 10.f);
        
        C2D_DrawRectSolid(x, 0.f, 0.f, console.texts[0].width * 0.5f, line_height, C2D_Color32(255, 255, 255, 255));
        C2D_DrawText(&console.texts[0], 0, x, 0.f, 0.f, 0.5f, 0.5f);
    }
}

void DebugConsole_ToggleEnabled() {
    console.enabled = !console.enabled;
}
