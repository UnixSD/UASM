#pragma once

#include <stdint.h>
#include <stdbool.h>

#define __ASSEMBLY__
#define __UASM_ASM__
#define __UASM_VERSION__ 0.1
#if defined(__x86_64__)
    #define __arch__ 8664
#elif defined(__i386__) || defined(_M_IX86)
    #define __arch__ 8632
#elif defined(__aarch64__)
    #define __arch__ 14732
#elif defined(__arm__) || defined(_M_ARM)
    #define __arch__ 14764
#else
    #define __arch__ 0
#endif

#if defined(_WIN64) || defined(_WIN32)
    #define __OS_BASE__ 1
#elif defined(__linux__)
    #define __OS_BASE__ 0
#elif defined(__APPLE__) || defined(_APPLE_)
    #define __OS_BASE__ 3
#else
    #define __OS_BASE__ -1
#endif