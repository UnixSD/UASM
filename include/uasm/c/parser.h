#pragma once

#include <stdbool.h>

#include "lexer.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ASCII table:
q: 113
w: 119
e: 101
r: 114
t: 116
y: 121
u: 117
i: 105
o: 111
p: 112
a: 97
s: 115
d: 100
f: 102
g: 103
h: 104
j: 106
k: 107
l: 108
z: 122
x: 120
c: 99
v: 118
b: 98
n: 110
m: 109
%: 37
(: 40
): 41
/: 47
|: 124
!: 33
#: 35
~: 126
-: 45
.: 46
,: 44
;: 59
*/

#define UASM_PARSER_SYMOL_Q 113
#define UASM_PARSER_SYMOL_W 119
#define UASM_PARSER_SYMOL_E 101
#define UASM_PARSER_SYMOL_R 114
#define UASM_PARSER_SYMOL_T 116
#define UASM_PARSER_SYMOL_Y 121
#define UASM_PARSER_SYMOL_U 117
#define UASM_PARSER_SYMOL_I 105
#define UASM_PARSER_SYMOL_O 111
#define UASM_PARSER_SYMOL_P 112
#define UASM_PARSER_SYMOL_A 97
#define UASM_PARSER_SYMOL_S 115
#define UASM_PARSER_SYMOL_D 100
#define UASM_PARSER_SYMOL_F 102
#define UASM_PARSER_SYMOL_G 103
#define UASM_PARSER_SYMOL_H 104
#define UASM_PARSER_SYMOL_J 106
#define UASM_PARSER_SYMOL_K 107
#define UASM_PARSER_SYMOL_L 108
#define UASM_PARSER_SYMOL_Z 122
#define UASM_PARSER_SYMOL_X 120
#define UASM_PARSER_SYMOL_C 99
#define UASM_PARSER_SYMOL_V 118
#define UASM_PARSER_SYMOL_B 98
#define UASM_PARSER_SYMOL_N 110
#define UASM_PARSER_SYMOL_M 109

#define UASM_PARSER_SYMOL_PERCENT 37 // %
#define UASM_PARSER_SYMOL_LPAREN 40 // (
#define UASM_PARSER_SYMOL_RPAREN 41 // )
#define UASM_PARSER_SYMOL_SLASH 47 // /
#define UASM_PARSER_SYMOL_PIPE 124 // |
#define UASM_PARSER_SYMOL_EXCLAMATION 33  // !
#define UASM_PARSER_SYMOL_HASH 35 // #
#define UASM_PARSER_SYMOL_TILDE 126 // ~
#define UASM_PARSER_SYMOL_HYPHEN 45 // -
#define UASM_PARSER_SYMOL_DOT 46 // .
#define UASM_PARSER_SYMOL_COMMA 44 // ,
#define UASM_PARSER_SYMOL_SEMICOLON 59 // ;

int toInt_char(const char* c);
bool parser_asm(const char* keyword);

#ifdef __cplusplus
}
#endif
