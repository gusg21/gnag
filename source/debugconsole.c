#include "debugconsole.h"

#include <citro2d.h>

debug_console_t console;
C2D_TextBuf buffer;
C2D_Font font;

void DebugConsole_Init(gfxScreen_t screen) {
    console.enabled = true;
    console.screen = screen;

    buffer = C2D_TextBufNew(4096);

    font = C2D_FontLoadSystem(CFG_REGION_USA);
}

void DebugConsole_Print(const char* text) {
    size_t text_len = strlen(text);

    for (s32 line_index = DEBUG_CONSOLE_LINE_COUNT - 1; line_index > 0; line_index--) {
        memcpy(console.texts[line_index - 1], console.texts[line_index], DEBUG_CONSOLE_LINE_LENGTH);
    } 

    memcpy(console.texts[0], text, text_len);
}

void DebugConsole_Draw() {
    float line_height = 10.f;

	C2D_Text dynText;
    for (u32 line_index = 0; line_index < DEBUG_CONSOLE_LINE_COUNT; line_index++) {
        C2D_TextParse(&dynText, buffer, console.texts[line_index]);
	    C2D_TextOptimize(&dynText);
        C2D_DrawText(&dynText, 0, 0.f, (float)line_index * line_height, 0.f, 0.5f, 0.5f);
    }
}
