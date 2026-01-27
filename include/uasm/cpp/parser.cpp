#include "../c/parser.h"

inline int toInt_char(char c[]) 
{
    int result = 0;
    for (int i = 0; c[i] != '\0'; ++i) 
    {
        result = result * 10 + (c[i] - '0');
    }
    return result;
}

// Функция для проверки, является ли символ допустимым в инструкции ассемблера
inline bool isValidAsmChar(unsigned char c)
{
    // Буквы (a-z, A-Z): ASCII 65-90 (A-Z), 97-122 (a-z)
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
        return true;
    
    // Цифры (0-9): ASCII 48-57
    if (c >= 48 && c <= 57)
        return true;
    
    // Допустимые специальные символы: _ (95), . (46), : (58)
    if (c == 95 || c == 46 || c == 58)
        return true;
    
    return false;
}

// Проверка, что строка содержит только допустимые ASCII коды для инструкций ассемблера
bool parser_asm(const char* keyword) 
{
    // Проверка на пустую строку
    if (keyword == nullptr || keyword[0] == '\0')
        return false;
    
    // Проверка каждого символа в строке
    for (int i = 0; keyword[i] != '\0'; ++i)
    {
        unsigned char ch = (unsigned char)keyword[i];
        
        // Если символ не допустим, возвращаем false
        if (!isValidAsmChar(ch))
            return false;
    }
    
    // Все символы прошли проверку
    return true;
}