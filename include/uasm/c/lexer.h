#pragma once

#include "uasm/c/asm.h"

#if defined(__SYNTAX__)
#if defined(__UASM__) && (__UASM__ == 64)
static const char* registry[] = {
    "RAX", "RBX", "RCX",
    "RDX", "RSI", "RDI",
    "RBP", "RSP",
};
#else
static const char* registry[] = {};
#endif
#endif

