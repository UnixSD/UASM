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


#include "../lib/assembly.h"
#include "../lib/lexer.h"
using namespace std;

#define __TOKEN_VERF__(value) tokens.size() < value
#define __BASE_STRING__ string dest = tokens[1]; \
                        string src = tokens[2];

#define __BASE_REG__ string reg = tokens[1];

class Assembler {
private:
    #ifdef _UASM64_
    map<string, int> registers = {
        {registry[0], 0}, {registry[1], 1}, {registry[2], 2}, {registry[3], 3},
        {registry[4], 4}, {registry[5], 5}, {registry[6], 6}, {registry[7], 7},

        {registry[8], 8}, {registry[9], 9}, {registry[10], 10}, {registry[11], 11},
        {registry[12], 12}, {registry[13], 13}, {registry[14], 14}, {registry[15], 15},

        {registry[16], 0}, {registry[17], 1}, {registry[18], 2}, {registry[19], 3},
        {registry[20], 4}, {registry[21], 5}, {registry[22], 6}, {registry[23], 7},

        {registry[24], 0}, {registry[25], 1}, {registry[26], 2}, {registry[27], 3},
        {registry[28], 4}, {registry[29], 5}, {registry[30], 6}, {registry[31], 7},

        {registry[32], 8}, {registry[33], 9}, {registry[34], 10}, {registry[35], 11},
        {registry[36], 12}, {registry[37], 13}, {registry[38], 14}, {registry[39], 15},
        {registry[40], 0}, {registry[41], 1}, {registry[42], 2}, {registry[43], 3},
        {registry[44], 4}, {registry[45], 5}, {registry[46], 6}, {registry[47], 7},
        {registry[48], 0}, {registry[49], 1}, {registry[50], 2}, {registry[51], 3}
    };
    #elif defined(_UASM32_)
    map<string, int> registers = {
        {registry[0], 0}, {registry[1], 1}, {registry[2], 2}, {registry[3], 3},
        {registry[4], 4}, {registry[5], 5}, {registry[6], 6}, {registry[7], 7},
        {registry[8], 0}, {registry[9], 1}, {registry[10], 2}, {registry[11], 3},

        {registry[12], 4}, {registry[13], 5}, {registry[14], 6}, {registry[15], 7},
        {registry[16], 0}, {registry[17], 1}, {registry[18], 2}, {registry[19], 3},
        {registry[20], 4}, {registry[21], 5}, {registry[22], 6}, {registry[23], 7},
        {registry[24], 0}, {registry[25], 1}, {registry[26], 2}, {registry[27], 3}
    };
    #elif defined(_UASM16_)
    map<string, int> registers = {
        {registry[0], 0}, {registry[1], 1}, {registry[2], 2}, {registry[3], 3},
        {registry[4], 4}, {registry[5], 5}, {registry[6], 6}, {registry[7], 7},
        {registry[8], 0}, {registry[9], 1}, {registry[10], 2}, {registry[11], 3},

        {registry[12], 4}, {registry[13], 5}, {registry[14], 6}, {registry[15], 7},
        {registry[16], 0}, {registry[17], 1}, {registry[18], 2}, {registry[19], 3}
    };
    #endif

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
        else if (opcode == "XCHG") 
        {
        }

        return result;
    }

private:
    string assembleMov(const vector<string>& tokens) 
    {
        if (__TOKEN_VERF__(3)) return;

        __BASE_STRING

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
        if (__TOKEN_VERF__(3)) return;

        __BASE_STRING

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
        if (__TOKEN_VERF__(3)) return;

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
        if (__TOKENT_VERF__(3)) return;

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
        if (__TOKEN_VERF__(2)) return;

        __BASE_REG__
        if (registers.count(reg) && reg[0] == 'e') 
        {
            return toHex(0x40 + registers[reg], 2);
        }

        return "[FAILED]: unknown INC instruction";
    }

    string assembleDec(const vector<string>& tokens) 
    {
        if (__TOKEN_VERF__(2)) return;

        __BASE_REG__
        if (registers.count(reg) && reg[0] == 'e') 
        {
            return toHex(0x48 + registers[reg], 2);
        }

        return "[FAILED]: unknown DEC instruction";
    }

    string assemblePush(const vector<string>& tokens) 
    {
        if (__TOKEN_VERF__(2)) return;

        __BASE_REG__
        if (registers.count(reg) && reg[0] == 'e') 
        {
            return toHex(0x50 + registers[reg], 2);
        }

        return "[FAILED]: unknown PUSH instruction";
    }

    string assemblePop(const vector<string>& tokens) 
    {
        if (__TOKEN_VERF__(2)) return;

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
