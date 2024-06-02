#include "panic.h"

#ifdef __3DS__
#include <3ds.h>
#endif

static bool Panic_S_is_panicked = false;

void Panic_Panic() {
    Panic_S_is_panicked = true;

#ifdef __3DS__
    consoleInit(GFX_TOP, NULL);
#endif
}

bool Panic_IsPanicked() {
    return Panic_S_is_panicked;
}
