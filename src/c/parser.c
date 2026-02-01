#include "uasm/c/parser.h"

#include <ctype.h>

int toInt_char(const char* c)
{
    int result = 0;
    if (!c) {
        return 0;
    }
    for (int i = 0; c[i] != '\0'; ++i) {
        if (!isdigit((unsigned char)c[i])) {
            return result;
        }
        result = result * 10 + (c[i] - '0');
    }
    return result;
}

static bool is_valid_asm_char(unsigned char c)
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        return true;
    }
    if (c >= '0' && c <= '9') {
        return true;
    }
    if (c == '_' || c == '.' || c == ':' || c == '%') {
        return true;
    }
    return false;
}

bool parser_asm(const char* keyword)
{
    if (!keyword || keyword[0] == '\0') {
        return false;
    }

    for (int i = 0; keyword[i] != '\0'; ++i) {
        unsigned char ch = (unsigned char)keyword[i];
        if (!is_valid_asm_char(ch)) {
            return false;
        }
    }

    return true;
}
