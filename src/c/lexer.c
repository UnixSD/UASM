#include "uasm/c/lexer.h"

#if defined(_UASM16_)
const char* const registry[] = {
    "AX", "BX", "CX",
    "DX", "SI", "DI",
    "BP", "SP",
    "R8W", "R9W", "R10W",
    "R11W", "R12W", "R13W"
};
#elif defined(_UASM32_)
const char* const registry[] = {
    "EAX", "EBX", "ECX",
    "EDX", "ESI", "EDI",
    "EBP", "ESP",
    "EIP", "EFLAGS",
    "CS", "DS", "SS", "ES", "FS", "GS",
    "R8",  "R9",  "R10",
    "R11", "R12", "R13",
    "R14", "R15",
    "AX", "BX", "CX",
    "DX", "SI", "DI",
    "BP", "SP",
    "R8W", "R9W", "R10W",
    "R11W", "R12W", "R13W"
};
#else
const char* const registry[] = {
    "RAX", "RBX", "RCX",
    "RDX", "RSI", "RDI",
    "RBP", "RSP",
    "EAX", "EBX", "ECX",
    "EDX", "ESI", "EDI",
    "EBP", "ESP",
    "EIP", "EFLAGS",
    "CS", "DS", "SS", "ES", "FS", "GS",
    "R8",  "R9",  "R10",
    "R11", "R12", "R13",
    "R14", "R15",
    "AX", "BX", "CX",
    "DX", "SI", "DI",
    "BP", "SP",
    "R8W", "R9W", "R10W",
    "R11W", "R12W", "R13W"
};
#endif

const size_t REGISTRY_COUNT = sizeof(registry) / sizeof(registry[0]);

const char* const syntax[] = {
    "%INCLUDE", "%IF", "%ELIF", "%DEF", "%ENDIF",
    "MOV", "XCHG", "PUSH",
    "POP", "ADD", "SUB",
    "INC", "DEC",
    "MUL", "IMUL", "DIV",
    "IDIV", "AND", "OR",
    "XOR", "NOT", "SHL",
    "SHR", "CMP", "TEST",
    "JMP", "JE", "JZ",
    "JNE", "JNZ", "JG", "JNLE",
    "JL", "JNGE", "JGE",
    "JLE", "CALL", "RET",
    "NOP", "INT", "HLT",
    "MOVSB", "STOSB",
    "LODSB", "LEA", "CLC", "STC",
    "CLD", "STD", "SAHF", "LAHF"
};

const size_t SYNTAX_COUNT = sizeof(syntax) / sizeof(syntax[0]);

const char* const directives[] = {
    "db", "dw", "dd", "dq",
    "section", "global", "extern",
    "bits", "end"
};

const size_t DIRECTIVE_COUNT = sizeof(directives) / sizeof(directives[0]);
