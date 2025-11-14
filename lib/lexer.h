/*  Copyright (C) 2025 UnixSD

   This file is part of UASM.

   UASM is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   UASM is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with UASM.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _UASM64_
const char* registry[] = {
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
const size_t REGISTRY_COUNT = sizeof(registry) / sizeof(registry[0]);
#elif _UASM32_
const char* registry[] = {
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
const size_t REGISTRY_COUNT = sizeof(registry) / sizeof(registry[0]);
#elif _UASM16_
const char* registry[] = {
    "AX", "BX", "CX",
    "DX", "SI", "DI",
    "BP", "SP",
    "R8W", "R9W", "R10W",
    "R11W", "R12W", "R13W"
};
const size_t REGISTRY_COUNT = sizeof(registry) / sizeof(registry[0]);
#endif

const char* syntax[] = {
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
