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


#include "uasm/cpp/assembly.h"
#include "uasm/cpp/lexer.h"

#include <cctype>
using namespace std;

#define __TOKEN_VERF__(value) (tokens.size() < static_cast<size_t>(value))
#define __BASE_STRING__ string dest = tokens[1]; \
                        string src = tokens[2];

#define __BASE_REG__ string reg = tokens[1];

namespace {
struct RegEntry {
    const char* name;
    int code;
};

static string lower_copy(const char* value)
{
    string out = value ? value : "";
    transform(out.begin(), out.end(), out.begin(), ::tolower);
    return out;
}

static const RegEntry kRegisterEntries[] = {
    {"eax", 0}, {"ecx", 1}, {"edx", 2}, {"ebx", 3},
    {"esp", 4}, {"ebp", 5}, {"esi", 6}, {"edi", 7},
    {"ax", 0}, {"cx", 1}, {"dx", 2}, {"bx", 3},
    {"sp", 4}, {"bp", 5}, {"si", 6}, {"di", 7},
    {"rax", 0}, {"rcx", 1}, {"rdx", 2}, {"rbx", 3},
    {"rsp", 4}, {"rbp", 5}, {"rsi", 6}, {"rdi", 7},
    {"r8", 8}, {"r9", 9}, {"r10", 10}, {"r11", 11},
    {"r12", 12}, {"r13", 13}, {"r14", 14}, {"r15", 15},
    {"r8w", 8}, {"r9w", 9}, {"r10w", 10}, {"r11w", 11},
    {"r12w", 12}, {"r13w", 13}
};
} // namespace

class Assembler {
private:
    map<string, int> registers;

    void init_registers()
    {
        for (const auto& entry : kRegisterEntries) {
            registers.emplace(lower_copy(entry.name), entry.code);
        }
    }

    string toLower(string s) 
    {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

    string trim(const string& str) 
    {
        size_t first = str.find_first_not_of(" \t");
        if (first == string::npos) return "";
        size_t last = str.find_last_not_of(" \t");
        return str.substr(first, (last - first + 1));
    }

    vector<string> tokenize(const string& line) 
    {
        vector<string> tokens;
        stringstream ss(line);
        string token;

        while (ss >> token) 
        {
            token.erase(remove(token.begin(), token.end(), ','), token.end());
            tokens.push_back(toLower(token));
        }

        return tokens;
    }

    bool isNumber(const string& s) 
    {
        if (s.empty()) return false;
        size_t start = (s[0] == '-') ? 1 : 0;
        for (size_t i = start; i < s.length(); i++) 
        {
            if (!isdigit(s[i]) && s[i] != 'x' &&
                (s[i] < 'a' || s[i] > 'f')) return false;
        }
        return true;
    }

    int parseNumber(const string& s) 
    {
        if (s.find("0x") == 0 || s.find("0X") == 0) 
        {
            return stoi(s, nullptr, 16);
        }
        return stoi(s);
    }

public:
    Assembler()
    {
        init_registers();
    }

    string assemble(const string& instruction) 
    {
        string line = trim(instruction);
        if (line.empty()) return "";

        vector<string> tokens = tokenize(line);
        if (tokens.empty()) return "";

        string opcode = tokens[0];
        string result;

        // MOV 
        if (opcode == "mov") {
            result = assembleMov(tokens);
        }
        // ADD 
        else if (opcode == "add") {
            result = assembleAdd(tokens);
        }
        // SUB 
        else if (opcode == "sub") 
        {
            result = assembleSub(tokens);
        }
        // INC 
        else if (opcode == "inc") 
        {
            result = assembleInc(tokens);
        }
        // DEC 
        else if (opcode == "dec") 
        {
            result = assembleDec(tokens);
        }
        // NOP
        else if (opcode == "nop") 
        {
            result = "90";
        }
        // RET
        else if (opcode == "ret") 
        {
            result = "C3";
        }
        // PUSH
        else if (opcode == "push") 
        {
            result = assemblePush(tokens);
        }
        // POP
        else if (opcode == "pop") 
        {
            result = assemblePop(tokens);
        }
        else if (opcode == "xchg") 
        {
            result = assembleXCHG(tokens);
        }

        return result;
    }

private:
    string assembleMov(const vector<string>& tokens) 
    {
        if (__TOKEN_VERF__(3)) return "[FAILED]: invalid MOV operands";

        __BASE_STRING__

        // MOV reg, reg
        if (registers.count(dest) && registers.count(src)) 
        {
            if (dest[0] == 'e') 
            { // 32-bit
                int modrm = 0xC0 | (registers[src] << 3) | registers[dest];
                return "89" + toHex(modrm, 2);
            }
            else if (dest.length() == 2 && dest[1] == 'x') 
            { // 16-bit
                int modrm = 0xC0 | (registers[src] << 3) | registers[dest];
                return "6689" + toHex(modrm, 2);
            }
        }

        // MOV reg, imm
        if (registers.count(dest) && isNumber(src)) 
        {
            int value = parseNumber(src);
            if (dest[0] == 'e') { // 32-bit
                return toHex(0xB8 + registers[dest], 2) + toHex(value, 8);
            }
            else if (dest.length() == 2 && dest[1] == 'x') 
            { // 16-bit
                return "66" + toHex(0xB8 + registers[dest], 2) + toHex(value, 4);
            }
            else 
            { // 8-bit
                return toHex(0xB0 + registers[dest], 2) + toHex(value, 2);
            }
        }

        return "[FAILED]: unknown MOV instruction";
    }

    string assembleAdd(const vector<string>& tokens) 
    {
        if (__TOKEN_VERF__(3)) return "[FAILED]: invalid ADD operands";

        __BASE_STRING__

        // ADD reg, imm
        if (registers.count(dest) && isNumber(src)) 
        {
            int value = parseNumber(src);
            if (dest == "eax") 
            {
                return "05" + toHex(value, 8);
            }
            if (dest[0] == 'e') 
            {
                int modrm = 0xC0 | registers[dest];
                return "81" + toHex(modrm, 2) + toHex(value, 8);
            }
        }

        return "[FAILED]: unknown ADD instruction";
    }

    string assembleXCHG(const vector<string>& tokens)
    {
        if (__TOKEN_VERF__(3)) return "[FAILED]: invalid XCHG operands";

        __BASE_STRING__
        // XCHG reg, reg
        if (registers.count(dest) && registers.count(src))
        {
            int modrm = 0xC0 | (registers[src] << 3) | registers[dest];
            return "87" + toHex(modrm, 2);
        }
       return "[FAILED]: unknown XCHG instruction";
    }

    string assembleSub(const vector<string>& tokens) 
    {
        if (__TOKEN_VERF__(3)) return "[FAILED]: invalid SUB operands";

        __BASE_STRING__

        // SUB reg, imm
        if (registers.count(dest) && isNumber(src)) 
        {
            int value = parseNumber(src);
            if (dest == "eax") 
            {
                return "2D" + toHex(value, 8);
            }
            if (dest[0] == 'e') 
            {
                int modrm = 0xE8 | registers[dest];
                return "81" + toHex(modrm, 2) + toHex(value, 8);
            }
        }

        return "[FAILED]: unknown SUB instruction";
    }

    string assembleInc(const vector<string>& tokens) 
    {
        if (__TOKEN_VERF__(2)) return "[FAILED]: invalid INC operands";

        __BASE_REG__
        if (registers.count(reg) && reg[0] == 'e') 
        {
            return toHex(0x40 + registers[reg], 2);
        }

        return "[FAILED]: unknown INC instruction";
    }

    string assembleDec(const vector<string>& tokens) 
    {
        if (__TOKEN_VERF__(2)) return "[FAILED]: invalid DEC operands";

        __BASE_REG__
        if (registers.count(reg) && reg[0] == 'e') 
        {
            return toHex(0x48 + registers[reg], 2);
        }

        return "[FAILED]: unknown DEC instruction";
    }

    string assemblePush(const vector<string>& tokens) 
    {
        if (__TOKEN_VERF__(2)) return "[FAILED]: invalid PUSH operands";

        __BASE_REG__
        if (registers.count(reg) && reg[0] == 'e') 
        {
            return toHex(0x50 + registers[reg], 2);
        }

        return "[FAILED]: unknown PUSH instruction";
    }

    string assemblePop(const vector<string>& tokens) 
    {
        if (__TOKEN_VERF__(2)) return "[FAILED]: invalid POP operands";

        __BASE_REG__
        if (registers.count(reg) && reg[0] == 'e') 
        {
            return toHex(0x58 + registers[reg], 2);
        }

        return "[FAILED]: unknown POP instruction";
    }

    string toHex(int value, int width) 
    {
        stringstream ss;
        ss << setfill('0') << setw(width) << hex << uppercase << value;
        return ss.str();
    }
};

string assemble_line(const string& instruction)
{
    static Assembler assembler;
    return assembler.assemble(instruction);
}
