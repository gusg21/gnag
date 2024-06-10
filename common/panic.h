#ifndef PANIC_H
#define PANIC_H

#include "_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

void Panic_Panic();
bool Panic_IsPanicked();

#ifdef __cplusplus
}
#endif

#endif  // PANIC_H