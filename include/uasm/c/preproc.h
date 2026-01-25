#pragma once
#include "uasm/c/asm.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// Максимальная длина имени макроса
#define MAX_MACRO_NAME 256
#define MAX_MACRO_VALUE 4096
#define MAX_INCLUDE_PATHS 32

// Структура для хранения макроса
typedef struct Macro {
    char name[MAX_MACRO_NAME];
    char value[MAX_MACRO_VALUE];
    bool has_args;
    char args[16][MAX_MACRO_NAME];  // Максимум 16 аргументов
    int arg_count;
    struct Macro* next;
} Macro;

// Структура для стека условной компиляции
typedef struct IfStack {
    bool condition;      // Результат условия
    bool else_seen;      // Был ли встречен #else
    bool active;         // Активна ли эта ветка
    struct IfStack* next;
} IfStack;

// Структура контекста препроцессора
typedef struct PreprocContext {
    Macro* macros;              // Список макросов
    IfStack* if_stack;          // Стек условной компиляции
    char include_paths[MAX_INCLUDE_PATHS][256];
    int include_path_count;
    FILE* output;               // Выходной файл
    int line_number;            // Текущий номер строки
    const char* filename;       // Текущий файл
} PreprocContext;

// Основные функции препроцессора
PreprocContext* preproc_create_context(void);
void preproc_destroy_context(PreprocContext* ctx);
void preproc_add_include_path(PreprocContext* ctx, const char* path);

// Обработка файла
int preproc_process_file(PreprocContext* ctx, const char* filename, FILE* output);
int preproc_process_stream(PreprocContext* ctx, FILE* input, const char* filename, FILE* output);

// Обработка директив
int preproc_handle_define(PreprocContext* ctx, const char* line);
int preproc_handle_undef(PreprocContext* ctx, const char* line);
int preproc_handle_include(PreprocContext* ctx, const char* line, FILE* output);
int preproc_handle_ifdef(PreprocContext* ctx, const char* line, bool is_ifndef);
int preproc_handle_if(PreprocContext* ctx, const char* line);
int preproc_handle_else(PreprocContext* ctx);
int preproc_handle_elif(PreprocContext* ctx, const char* line);
int preproc_handle_endif(PreprocContext* ctx);

// Работа с макросами
Macro* preproc_find_macro(PreprocContext* ctx, const char* name);
void preproc_add_macro(PreprocContext* ctx, const char* name, const char* value);
void preproc_remove_macro(PreprocContext* ctx, const char* name);

// Расширение макросов
char* preproc_expand_macros(PreprocContext* ctx, const char* line);

// Вспомогательные функции
bool preproc_evaluate_condition(PreprocContext* ctx, const char* expr);
bool preproc_is_defined(PreprocContext* ctx, const char* name);
int preproc_get_macro_value(PreprocContext* ctx, const char* name);
char* preproc_find_include_file(PreprocContext* ctx, const char* filename);
