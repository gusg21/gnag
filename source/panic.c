#include "panic.h"

#include <3ds.h>

static bool Panic_S_is_panicked = false;

void Panic_Panic() {
    Panic_S_is_panicked = true;

    consoleInit(GFX_TOP, NULL);
}

bool Panic_IsPanicked() {
    return Panic_S_is_panicked;
}
