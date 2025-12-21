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

#define __ERRNO 0x1
#define __WARN  0x2
#define __OK    0x3

#define __this_status 0x0

inline void __this_status_change(uint8_t status)
{
    #if status == 0x1
       #undef __this_status
       #define __this_status __ERRNO
    #elif status == 0x2
       #undef __this_status
       #define __this_status __WARN
    #elif status == 0x3
       #undef __this_status
       #define __this_status __OK
    #endif
}


typedef struct 
{
    char header_t;
    char subheader_t;
    bool comant_t;
} file_discription;

typedef struct
{
    char command_t;
    char registry_t;
    int value_t;
} command_discription;

#ifdef __ASSEMBLY__ 
#   if __OS_BASE__ == 0
#       define __elf
#   elif __OS_BASE__ == 1  
#       define __executabloe
#   elif __OS_BASE__ == 2
#       define __dmg
#   endif
#endif 

#undef __Obj_file