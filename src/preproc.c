#include "preproc.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Пропуск пробелов в строке
static void skip_whitespace(const char** str) {
    while (**str && isspace((unsigned char)**str)) {
        (*str)++;
    }
}

// Пропуск пробелов и табуляций
static void skip_spaces(const char** str) {
    while (**str && (**str == ' ' || **str == '\t')) {
        (*str)++;
    }
}

// Чтение идентификатора
static char* read_identifier(const char** str) {
    skip_spaces(str);
    const char* start = *str;
    if (!isalpha((unsigned char)**str) && **str != '_') {
        return NULL;
    }
    while (isalnum((unsigned char)**str) || **str == '_') {
        (*str)++;
    }
    size_t len = *str - start;
    char* id = (char*)malloc(len + 1);
    if (!id) return NULL;  // Проверка на успешное выделение памяти
    strncpy(id, start, len);
    id[len] = '\0';
    return id;
}

// Чтение строки до конца строки или до указанного символа
static char* read_until(const char** str, char delim) {
    skip_spaces(str);
    const char* start = *str;
    while (**str && **str != delim && **str != '\n' && **str != '\r') {
        (*str)++;
    }
    size_t len = *str - start;
    char* result = (char*)malloc(len + 1);
    if (!result) return NULL;  // Проверка на успешное выделение памяти
    strncpy(result, start, len);
    result[len] = '\0';
    if (**str == delim) (*str)++;
    return result;
}

// Удаление пробелов в начале и конце строки
static char* trim_string(char* str) {
    if (!str) return NULL;
    
    // Удаление пробелов в начале
    while (isspace((unsigned char)*str)) {
        str++;
    }
    
    // Удаление пробелов в конце
    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }
    
    return str;
}

// Создание контекста препроцессора
PreprocContext* preproc_create_context(void) {
    PreprocContext* ctx = (PreprocContext*)calloc(1, sizeof(PreprocContext));
    if (!ctx) return NULL;
    
    ctx->macros = NULL;
    ctx->if_stack = NULL;
    ctx->include_path_count = 0;
    ctx->output = NULL;
    ctx->line_number = 0;
    ctx->filename = NULL;
    
    return ctx;
}

// Уничтожение контекста
void preproc_destroy_context(PreprocContext* ctx) {
    if (!ctx) return;
    
    // Освобождение макросов
    Macro* macro = ctx->macros;
    while (macro) {
        Macro* next = macro->next;
        free(macro);
        macro = next;
    }
    
    // Освобождение стека условной компиляции
    IfStack* if_item = ctx->if_stack;
    while (if_item) {
        IfStack* next = if_item->next;
        free(if_item);
        if_item = next;
    }
    
    free(ctx);
}

// Добавление пути для поиска include файлов
void preproc_add_include_path(PreprocContext* ctx, const char* path) {
    if (!ctx || !path || ctx->include_path_count >= MAX_INCLUDE_PATHS) {
        return;
    }
    
    if (strlen(path) > 255) {
        fprintf(stderr, "Warning: Path is too long, truncating: %s\n", path);
    }
    
    strncpy(ctx->include_paths[ctx->include_path_count], path, 255);
    ctx->include_paths[ctx->include_path_count][255] = '\0';
    ctx->include_path_count++;
}

// Поиск макроса
Macro* preproc_find_macro(PreprocContext* ctx, const char* name) {
    if (!ctx || !name) return NULL;
    
    Macro* macro = ctx->macros;
    while (macro) {
        if (strcmp(macro->name, name) == 0) {
            return macro;
        }
        macro = macro->next;
    }
    
    return NULL;
}

// Добавление макроса
void preproc_add_macro(PreprocContext* ctx, const char* name, const char* value) {
    if (!ctx || !name) return;
    
    // Удаляем существующий макрос с таким именем
    preproc_remove_macro(ctx, name);
    
    Macro* macro = (Macro*)calloc(1, sizeof(Macro));
    if (!macro) return;  // Проверка на успешное выделение памяти
    
    strncpy(macro->name, name, MAX_MACRO_NAME - 1);
    macro->name[MAX_MACRO_NAME - 1] = '\0';
    
    if (value) {
        strncpy(macro->value, value, MAX_MACRO_VALUE - 1);
        macro->value[MAX_MACRO_VALUE - 1] = '\0';
    } else {
        macro->value[0] = '\0';
    }
    
    macro->has_args = false;
    macro->arg_count = 0;
    macro->next = ctx->macros;
    ctx->macros = macro;
}

// Удаление макроса
void preproc_remove_macro(PreprocContext* ctx, const char* name) {
    if (!ctx || !name) return;
    
    Macro* prev = NULL;
    Macro* macro = ctx->macros;
    
    while (macro) {
        if (strcmp(macro->name, name) == 0) {
            if (prev) {
                prev->next = macro->next;
            } else {
                ctx->macros = macro->next;
            }
            free(macro);
            return;
        }
        prev = macro;
        macro = macro->next;
    }
}

// Проверка, определен ли макрос
bool preproc_is_defined(PreprocContext* ctx, const char* name) {
    return preproc_find_macro(ctx, name) != NULL;
}

// Получение значения макроса как числа
int preproc_get_macro_value(PreprocContext* ctx, const char* name) {
    Macro* macro = preproc_find_macro(ctx, name);
    if (!macro || !macro->value[0]) {
        return 0;
    }
    return atoi(macro->value);
}

// Удаление макроса
void preproc_remove_macro(PreprocContext* ctx, const char* name) {
    if (!ctx || !name) return;
    
 #include "preproc.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Пропуск пробелов в строке
static void skip_whitespace(const char** str) {
    while (**str && isspace((unsigned char)**str)) {
        (*str)++;
    }
}

// Пропуск пробелов и табуляций
static void skip_spaces(const char** str) {
    while (**str && (**str == ' ' || **str == '\t')) {
        (*str)++;
    }
}

// Чтение идентификатора
static char* read_identifier(const char** str) {
    skip_spaces(str);
    const char* start = *str;
    if (!isalpha((unsigned char)**str) && **str != '_') {
        return NULL;
    }
    while (isalnum((unsigned char)**str) || **str == '_') {
        (*str)++;
    }
    size_t len = *str - start;
    char* id = (char*)malloc(len + 1);
    if (!id) return NULL;  // Проверка на успешное выделение памяти
    strncpy(id, start, len);
    id[len] = '\0';
    return id;
}

// Чтение строки до конца строки или до указанного символа
static char* read_until(const char** str, char delim) {
    skip_spaces(str);
    const char* start = *str;
    while (**str && **str != delim && **str != '\n' && **str != '\r') {
        (*str)++;
    }
    size_t len = *str - start;
    char* result = (char*)malloc(len + 1);
    if (!result) return NULL;  // Проверка на успешное выделение памяти
    strncpy(result, start, len);
    result[len] = '\0';
    if (**str == delim) (*str)++;
    return result;
}

// Удаление пробелов в начале и конце строки
static char* trim_string(char* str) {
    if (!str) return NULL;
    
    // Удаление пробелов в начале
    while (isspace((unsigned char)*str)) {
        str++;
    }
    
    // Удаление пробелов в конце
    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }
    
    return str;
}

// Создание контекста препроцессора
PreprocContext* preproc_create_context(void) {
    PreprocContext* ctx = (PreprocContext*)calloc(1, sizeof(PreprocContext));
    if (!ctx) return NULL;
    
    ctx->macros = NULL;
    ctx->if_stack = NULL;
    ctx->include_path_count = 0;
    ctx->output = NULL;
    ctx->line_number = 0;
    ctx->filename = NULL;
    
    return ctx;
}

// Уничтожение контекста
void preproc_destroy_context(PreprocContext* ctx) {
    if (!ctx) return;
    
    // Освобождение макросов
    Macro* macro = ctx->macros;
    while (macro) {
        Macro* next = macro->next;
        free(macro);
        macro = next;
    }
    
    // Освобождение стека условной компиляции
    IfStack* if_item = ctx->if_stack;
    while (if_item) {
        IfStack* next = if_item->next;
        free(if_item);
        if_item = next;
    }
    
    free(ctx);
}

// Добавление пути для поиска include файлов
void preproc_add_include_path(PreprocContext* ctx, const char* path) {
    if (!ctx || !path || ctx->include_path_count >= MAX_INCLUDE_PATHS) {
        return;
    }
    
    if (strlen(path) > 255) {
        fprintf(stderr, "Warning: Path is too long, truncating: %s\n", path);
    }
    
    strncpy(ctx->include_paths[ctx->include_path_count], path, 255);
    ctx->include_paths[ctx->include_path_count][255] = '\0';
    ctx->include_path_count++;
}

// Поиск макроса
Macro* preproc_find_macro(PreprocContext* ctx, const char* name) {
    if (!ctx || !name) return NULL;
    
    Macro* macro = ctx->macros;
    while (macro) {
        if (strcmp(macro->name, name) == 0) {
            return macro;
        }
        macro = macro->next;
    }
    
    return NULL;
}

// Добавление макроса
void preproc_add_macro(PreprocContext* ctx, const char* name, const char* value) {
    if (!ctx || !name) return;
    
    // Удаляем существующий макрос с таким именем
    preproc_remove_macro(ctx, name);
    
    Macro* macro = (Macro*)calloc(1, sizeof(Macro));
    if (!macro) return;  // Проверка на успешное выделение памяти
    
    strncpy(macro->name, name, MAX_MACRO_NAME - 1);
    macro->name[MAX_MACRO_NAME - 1] = '\0';
    
    if (value) {
        strncpy(macro->value, value, MAX_MACRO_VALUE - 1);
        macro->value[MAX_MACRO_VALUE - 1] = '\0';
    } else {
        macro->value[0] = '\0';
    }
    
    macro->has_args = false;
    macro->arg_count = 0;
    macro->next = ctx->macros;
    ctx->macros = macro;
}

// Удаление макроса
void preproc_remove_macro(PreprocContext* ctx, const char* name) {
    if (!ctx || !name) return;
    
    Macro* prev = NULL;
    Macro* macro = ctx->macros;
    
    while (macro) {
        if (strcmp(macro->name, name) == 0) {
            if (prev) {
                prev->next = macro->next;
            } else {
                ctx->macros = macro->next;
            }
            free(macro);
            return;
        }
        prev = macro;
        macro = macro->next;
    }
}

// Проверка, определен ли макрос
bool preproc_is_defined(PreprocContext* ctx, const char* name) {
    return preproc_find_macro(ctx, name) != NULL;
}

// Получение значения макроса как числа
int preproc_get_macro_value(PreprocContext* ctx, const char* name) {
    Macro* macro = preproc_find_macro(ctx, name);
    if (!macro || !macro->value[0]) {
        return 0;
    }
    return atoi(macro->value);
}

// Обработка директивы %DEF или #define
int preproc_handle_define(PreprocContext* ctx, const char* line) {
    if (!ctx || !line) return -1;
    
    const char* p = line;
    skip_spaces(&p);
    
    // Пропускаем "%DEF" или "#define"
    if (strncmp(p, "DEF", 3) == 0) {
        p += 3;
    } else if (strncmp(p, "define", 6) == 0) {
        p += 6;
    } else {
        return -1;
    }
    
    // Читаем имя макроса
    char* name = read_identifier(&p);
    if (!name) {
        return -1;
    }
    
    // Проверяем, есть ли аргументы (макрос-функция)
    skip_spaces(&p);
    bool has_args = (*p == '(');
    
    char* value = NULL;
    if (has_args) {
        p++; // Пропускаем '('
        Macro* macro = (Macro*)calloc(1, sizeof(Macro));
        if (!macro) {
            free(name);
            return -1;
        }
        
        strncpy(macro->name, name, MAX_MACRO_NAME - 1);
        macro->has_args = true;
        macro->arg_count = 0;
        
        // Читаем аргументы
        while (*p && *p != ')' && macro->arg_count < 16) {
            skip_spaces(&p);
            if (*p == ')') break;
            
            char* arg = read_identifier(&p);
            if (!arg) break;
            
            strncpy(macro->args[macro->arg_count], arg, MAX_MACRO_NAME - 1);
            macro->arg_count++;
            free(arg);
            
            skip_spaces(&p);
            if (*p == ',') p++;
        }
        
        if (*p == ')') p++;
        skip_spaces(&p);
        
        // Читаем значение
        value = read_until(&p, '\0');
        if (value) {
            trim_string(value);
            strncpy(macro->value, value, MAX_MACRO_VALUE - 1);
            free(value);
        }
        
        macro->next = ctx->macros;
        ctx->macros = macro;
        free(name);
        return 0;
    } else {
        // Простой макрос
        value = read_until(&p, '\0');
        if (value) {
            trim_string(value);
            preproc_add_macro(ctx, name, value);
            free(value);
        } else {
            preproc_add_macro(ctx, name, NULL);
        }
        free(name);
        return 0;
    }
}

// Обработка директивы #undef
int preproc_handle_undef(PreprocContext* ctx, const char* line) {
    if (!ctx || !line) return -1;
    
    const char* p = line;
    skip_spaces(&p);
    
    if (strncmp(p, "undef", 5) != 0) return -1;
    p += 5;
    
    char* name = read_identifier(&p);
    if (!name) return -1;
    
    preproc_remove_macro(ctx, name);
    free(name);
    return 0;
}

// Поиск файла для включения
char* preproc_find_include_file(PreprocContext* ctx, const char* filename) {
    if (!ctx || !filename) return NULL;
    
    // Проверяем, существует ли файл напрямую
    FILE* test = fopen(filename, "r");
    if (test) {
        fclose(test);
        char* result = (char*)malloc(strlen(filename) + 1);
        strcpy(result, filename);
        return result;
    }
    
    // Проверяем пути из include_paths
    for (int i = 0; i < ctx->include_path_count; i++) {
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/%s", ctx->include_paths[i], filename);
        
        test = fopen(full_path, "r");
        if (test) {
            fclose(test);
            char* result = (char*)malloc(strlen(full_path) + 1);
            strcpy(result, full_path);
            return result;
        }
    }
    
    return NULL;
}

// Обработка директивы %INCLUDE или #include
int preproc_handle_include(PreprocContext* ctx, const char* line, FILE* output) {
    if (!ctx || !line || !output) return -1;
    
    const char* p = line;
    skip_spaces(&p);
    
    bool is_percent = false;
    if (strncmp(p, "INCLUDE", 7) == 0) {
        p += 7;
        is_percent = true;
    } else if (strncmp(p, "include", 7) == 0) {
        p += 7;
    } else {
        return -1;
    }
    skip_spaces(&p);
    
    char* filename = NULL;
    bool system_include = false;
    
    if (*p == '<') {
        system_include = true;
        p++;
        filename = read_until(&p, '>');
    } else if (*p == '"') {
        p++;
        filename = read_until(&p, '"');
    } else {
        return -1;
    }
    
    if (!filename) return -1;
    
    char* full_path = NULL;
    
    // Для системных include (<>) ищем в /usr/include
    if (system_include) {
        char system_path[512];
        snprintf(system_path, sizeof(system_path), "/usr/include/%s", filename);
        FILE* test = fopen(system_path, "r");
        if (test) {
            fclose(test);
            full_path = (char*)malloc(strlen(system_path) + 1);
            strcpy(full_path, system_path);
        }
    }
    
    // Если не найден как системный, ищем в обычных путях
    if (!full_path) {
        full_path = preproc_find_include_file(ctx, filename);
    }
    
    if (!full_path) {
        // Если системный include не найден, просто пропускаем
        if (system_include) {
            free(filename);
            return 0;
        }
        free(filename);
        return -1;
    }
    
    // Обрабатываем включенный файл
    FILE* include_file = fopen(full_path, "r");
    if (!include_file) {
        free(filename);
        free(full_path);
        return -1;
    }
    
    const char* old_filename = ctx->filename;
    int old_line = ctx->line_number;
    
    ctx->filename = full_path;
    ctx->line_number = 0;
    
    int result = preproc_process_stream(ctx, include_file, full_path, output);
    
    fclose(include_file);
    ctx->filename = old_filename;
    ctx->line_number = old_line;
    
    free(filename);
    free(full_path);
    return result;
}

// Обработка директивы %IFDEF / %IFNDEF или #ifdef / #ifndef
int preproc_handle_ifdef(PreprocContext* ctx, const char* line, bool is_ifndef) {
    if (!ctx || !line) return -1;
    
    const char* p = line;
    skip_spaces(&p);
    
    if (is_ifndef) {
        if (strncmp(p, "IFNDEF", 6) == 0) {
            p += 6;
        } else if (strncmp(p, "ifndef", 6) == 0) {
            p += 6;
        } else {
            return -1;
        }
    } else {
        if (strncmp(p, "IFDEF", 5) == 0) {
            p += 5;
        } else if (strncmp(p, "ifdef", 5) == 0) {
            p += 5;
        } else {
            return -1;
        }
    }
    
    char* name = read_identifier(&p);
    if (!name) return -1;
    
    bool defined = preproc_is_defined(ctx, name);
    bool condition = is_ifndef ? !defined : defined;
    
    // Проверяем, активна ли родительская ветка
    bool parent_active = true;
    if (ctx->if_stack) {
        parent_active = ctx->if_stack->active;
    }
    
    IfStack* if_item = (IfStack*)calloc(1, sizeof(IfStack));
    if (!if_item) {
        free(name);
        return -1;
    }
    
    if_item->condition = condition;
    if_item->else_seen = false;
    if_item->active = parent_active && condition;
    if_item->next = ctx->if_stack;
    ctx->if_stack = if_item;
    
    free(name);
    return 0;
}

// Обработка директивы %IF или #if
int preproc_handle_if(PreprocContext* ctx, const char* line) {
    if (!ctx || !line) return -1;
    
    const char* p = line;
    skip_spaces(&p);
    
    if (strncmp(p, "IF", 2) == 0) {
        p += 2;
    } else if (strncmp(p, "if", 2) == 0) {
        p += 2;
    } else {
        return -1;
    }
    skip_spaces(&p);
    
    bool condition = preproc_evaluate_condition(ctx, p);
    
    // Проверяем, активна ли родительская ветка
    bool parent_active = true;
    if (ctx->if_stack) {
        parent_active = ctx->if_stack->active;
    }
    
    IfStack* if_item = (IfStack*)calloc(1, sizeof(IfStack));
    if (!if_item) return -1;
    
    if_item->condition = condition;
    if_item->else_seen = false;
    if_item->active = parent_active && condition;
    if_item->next = ctx->if_stack;
    ctx->if_stack = if_item;
    
    return 0;
}

// Обработка директивы #else
int preproc_handle_else(PreprocContext* ctx) {
    if (!ctx || !ctx->if_stack) return -1;
    
    if (ctx->if_stack->else_seen) {
        return -1; // #else уже был
    }
    
    ctx->if_stack->else_seen = true;
    
    // Проверяем, активна ли родительская ветка
    bool parent_active = true;
    IfStack* parent = ctx->if_stack->next;
    if (parent) {
        parent_active = parent->active;
    }
    
    // Активируем ветку, если условие было ложным и родитель активен
    ctx->if_stack->active = parent_active && !ctx->if_stack->condition;
    
    return 0;
}

// Обработка директивы #elif
int preproc_handle_elif(PreprocContext* ctx, const char* line) {
    if (!ctx || !line || !ctx->if_stack) return -1;
    
    if (ctx->if_stack->else_seen) {
        return -1; // #else уже был, #elif недопустим
    }
    
    const char* p = line;
    skip_spaces(&p);
    
    if (strncmp(p, "elif", 4) != 0) return -1;
    p += 4;
    skip_spaces(&p);
    
    bool condition = preproc_evaluate_condition(ctx, p);
    
    // Проверяем, активна ли родительская ветка
    bool parent_active = true;
    IfStack* parent = ctx->if_stack->next;
    if (parent) {
        parent_active = parent->active;
    }
    
    // Активируем ветку, если предыдущее условие было ложным, новое истинно и родитель активен
    ctx->if_stack->active = parent_active && !ctx->if_stack->condition && condition;
    ctx->if_stack->condition = condition; // Обновляем условие
    
    return 0;
}

// Обработка директивы #endif
int preproc_handle_endif(PreprocContext* ctx) {
    if (!ctx || !ctx->if_stack) return -1;
    
    IfStack* top = ctx->if_stack;
    ctx->if_stack = top->next;
    free(top);
    
    return 0;
}

// Проверка, активна ли текущая ветка условной компиляции
static bool is_currently_active(PreprocContext* ctx) {
    if (!ctx->if_stack) return true;
    
    IfStack* item = ctx->if_stack;
    while (item) {
        if (!item->active) return false;
        item = item->next;
    }
    return true;
}

// Вычисление условия для #if
bool preproc_evaluate_condition(PreprocContext* ctx, const char* expr) {
    if (!ctx || !expr) return false;
    
    // Простая реализация: проверяем, определен ли макрос и его значение
    const char* p = expr;
    skip_spaces(&p);
    
    // Проверка на defined(NAME)
    if (strncmp(p, "defined", 7) == 0) {
        p += 7;
        skip_spaces(&p);
        if (*p == '(') {
            p++;
            char* name = read_identifier(&p);
            if (name) {
                bool result = preproc_is_defined(ctx, name);
                free(name);
                return result;
            }
        }
    }
    
    // Проверка числового значения
    char* name = read_identifier(&p);
    if (name) {
        int value = preproc_get_macro_value(ctx, name);
        free(name);
        return value != 0;
    }
    
    // Попытка интерпретировать как число
    int value = atoi(expr);
    return value != 0;
}

// Расширение макросов в строке (упрощенная версия)
char* preproc_expand_macros(PreprocContext* ctx, const char* line) {
    if (!ctx || !line) return NULL;
    
    // Простая реализация: заменяем только простые макросы без аргументов
    char* result = (char*)malloc(strlen(line) + 4096);
    if (!result) return NULL;
    
    strcpy(result, line);
    
    // Ищем имена макросов в строке
    Macro* macro = ctx->macros;
    while (macro && !macro->has_args) {
        char* pos = strstr(result, macro->name);
        while (pos) {
            // Проверяем, что это отдельное слово
            bool is_word = true;
            if (pos > result && (isalnum((unsigned char)pos[-1]) || pos[-1] == '_')) {
                is_word = false;
            }
            if (isalnum((unsigned char)pos[strlen(macro->name)]) || pos[strlen(macro->name)] == '_') {
                is_word = false;
            }
            
            if (is_word) {
                // Заменяем макрос
                size_t before_len = pos - result;
                size_t after_len = strlen(pos + strlen(macro->name));
                size_t new_len = before_len + strlen(macro->value) + after_len;
                
                char* new_result = (char*)realloc(result, new_len + 1);
                if (!new_result) {
                    free(result);
                    return NULL;
                }
                result = new_result;
                
                memmove(result + before_len + strlen(macro->value),
                       pos + strlen(macro->name),
                       after_len + 1);
                memcpy(result + before_len, macro->value, strlen(macro->value));
                
                pos = strstr(result + before_len + strlen(macro->value), macro->name);
            } else {
                pos = strstr(pos + 1, macro->name);
            }
        }
        macro = macro->next;
    }
    
    return result;
}

// Чтение строки из файла (замена getline для совместимости)
static int read_line(FILE* file, char** line, size_t* line_size) {
    if (!file || !line) return -1;
    
    if (!*line) {
        *line_size = 256;
        *line = (char*)malloc(*line_size);
        if (!*line) return -1;
    }
    
    size_t pos = 0;
    int c;
    
    while ((c = fgetc(file)) != EOF) {
        if (pos >= *line_size - 1) {
            *line_size *= 2;
            char* new_line = (char*)realloc(*line, *line_size);
            if (!new_line) return -1;
            *line = new_line;
        }
        
        (*line)[pos++] = (char)c;
        
        if (c == '\n') {
            break;
        }
    }
    
    if (pos == 0 && c == EOF) {
        return -1;
    }
    
    (*line)[pos] = '\0';
    return (int)pos;
}

// Обработка потока
int preproc_process_stream(PreprocContext* ctx, FILE* input, const char* filename, FILE* output) {
    if (!ctx || !input || !output) return -1;
    
    char* line = NULL;
    size_t line_size = 0;
    ctx->line_number = 0;
    
    while (read_line(input, &line, &line_size) != -1) {
        ctx->line_number++;
        
        // Удаляем символ новой строки
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }
        if (len > 0 && line[len - 1] == '\r') {
            line[len - 1] = '\0';
        }
        
        const char* p = line;
        skip_spaces(&p);
        
        // Пропускаем пустые строки и комментарии
        if (*p == '\0' || (*p == ';' && p == line)) {
            if (is_currently_active(ctx)) {
                fprintf(output, "%s\n", line);
            }
            continue;
        }
        
        // Обработка директив препроцессора (% вместо #)
        if (*p == '%') {
            p++;
            skip_spaces(&p);
            
            if (strncmp(p, "DEF", 3) == 0) {
                preproc_handle_define(ctx, p);
            } else if (strncmp(p, "undef", 5) == 0) {
                preproc_handle_undef(ctx, p);
            } else if (strncmp(p, "INCLUDE", 7) == 0) {
                if (is_currently_active(ctx)) {
                    preproc_handle_include(ctx, p, output);
                }
            } else if (strncmp(p, "IFDEF", 5) == 0) {
                preproc_handle_ifdef(ctx, p, false);
            } else if (strncmp(p, "IFNDEF", 6) == 0) {
                preproc_handle_ifdef(ctx, p, true);
            } else if (strncmp(p, "IF", 2) == 0) {
                preproc_handle_if(ctx, p);
            } else if (strncmp(p, "else", 4) == 0 || strncmp(p, "ELSE", 4) == 0) {
                preproc_handle_else(ctx);
            } else if (strncmp(p, "elif", 4) == 0 || strncmp(p, "ELIF", 4) == 0) {
                preproc_handle_elif(ctx, p);
            } else if (strncmp(p, "endif", 5) == 0 || strncmp(p, "ENDIF", 5) == 0) {
                preproc_handle_endif(ctx);
            } else if (strncmp(p, "ERR", 3) == 0) {
                // Обработка %ERR
                p += 3;
                skip_spaces(&p);
                char* message = read_until(&p, '\0');
                if (message && is_currently_active(ctx)) {
                    fprintf(stderr, "Error: %s\n", message);
                    free(message);
                    return -1;
                }
                if (message) free(message);
            }
            
            // Директивы препроцессора не выводятся в выходной файл
            continue;
        }
        
        // Обычная строка кода
        if (is_currently_active(ctx)) {
            // Расширяем макросы
            char* expanded = preproc_expand_macros(ctx, line);
            if (expanded) {
                fprintf(output, "%s\n", expanded);
                free(expanded);
            } else {
                fprintf(output, "%s\n", line);
            }
        }
    }
    
    free(line);
    return 0;
}

// Обработка файла
int preproc_process_file(PreprocContext* ctx, const char* filename, FILE* output) {
    if (!ctx || !filename || !output) return -1;
    
    FILE* input = fopen(filename, "r");
    if (!input) {
        return -1;
    }
    
    ctx->filename = filename;
    int result = preproc_process_stream(ctx, input, filename, output);
    
    fclose(input);
    return result;
}

