#include "uasm/cpp/syntax.h"

#include "uasm/c/lexer.h"

#include <cctype>
#include <cstring>

static bool equals_case_insensitive(const char* a, const char* b)
{
    if (!a || !b) {
        return false;
    }
    while (*a && *b) {
        if (std::tolower(static_cast<unsigned char>(*a)) !=
            std::tolower(static_cast<unsigned char>(*b))) {
            return false;
        }
        ++a;
        ++b;
    }
    return *a == '\0' && *b == '\0';
}

void keyword_check(char* token)
{
    if (!token) {
        return;
    }
    for (char* p = token; *p; ++p) {
        *p = static_cast<char>(std::tolower(static_cast<unsigned char>(*p)));
    }
}

bool is_register(char* token)
{
    if (!token) {
        return false;
    }
    for (size_t i = 0; i < REGISTRY_COUNT; ++i) {
        if (equals_case_insensitive(token, registry[i])) {
            return true;
        }
    }
    return false;
}

bool is_directive(char* token)
{
    if (!token) {
        return false;
    }
    for (size_t i = 0; i < DIRECTIVE_COUNT; ++i) {
        if (equals_case_insensitive(token, directives[i])) {
            return true;
        }
    }
    return false;
}
