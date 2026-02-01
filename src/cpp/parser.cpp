#include "uasm/cpp/parser.h"

#include "uasm/c/lexer.h"
#include "uasm/c/parser.h"

#include <cctype>
#include <string>
#include <vector>

namespace uasm {
namespace {

bool equals_case_insensitive(const std::string& value, const char* token)
{
    if (!token) {
        return false;
    }
    size_t i = 0;
    for (; i < value.size() && token[i] != '\0'; ++i) {
        if (std::tolower(static_cast<unsigned char>(value[i])) !=
            std::tolower(static_cast<unsigned char>(token[i]))) {
            return false;
        }
    }
    return i == value.size() && token[i] == '\0';
}

std::string trim_copy(const std::string& value)
{
    size_t start = 0;
    while (start < value.size() && std::isspace(static_cast<unsigned char>(value[start]))) {
        ++start;
    }
    size_t end = value.size();
    while (end > start && std::isspace(static_cast<unsigned char>(value[end - 1]))) {
        --end;
    }
    return value.substr(start, end - start);
}

std::string strip_comment(const std::string& value)
{
    bool in_single = false;
    bool in_double = false;
    for (size_t i = 0; i < value.size(); ++i) {
        char c = value[i];
        if (c == '\'' && !in_double) {
            in_single = !in_single;
        } else if (c == '"' && !in_single) {
            in_double = !in_double;
        } else if (c == ';' && !in_single && !in_double) {
            return value.substr(0, i);
        }
    }
    return value;
}

bool is_instruction_token(const std::string& token)
{
    for (size_t i = 0; i < SYNTAX_COUNT; ++i) {
        if (syntax[i][0] == '%') {
            continue;
        }
        if (equals_case_insensitive(token, syntax[i])) {
            return true;
        }
    }
    return false;
}

bool is_directive_token(const std::string& token)
{
    if (!token.empty() && (token[0] == '%' || token[0] == '#')) {
        return true;
    }
    for (size_t i = 0; i < DIRECTIVE_COUNT; ++i) {
        if (equals_case_insensitive(token, directives[i])) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> split_operands(const std::string& value)
{
    std::vector<std::string> out;
    std::string current;
    int bracket_depth = 0;
    int paren_depth = 0;
    bool in_single = false;
    bool in_double = false;

    for (size_t i = 0; i < value.size(); ++i) {
        char c = value[i];
        if (c == '\'' && !in_double) {
            in_single = !in_single;
        } else if (c == '"' && !in_single) {
            in_double = !in_double;
        }

        if (!in_single && !in_double) {
            if (c == '[') {
                ++bracket_depth;
            } else if (c == ']') {
                bracket_depth = bracket_depth > 0 ? bracket_depth - 1 : 0;
            } else if (c == '(') {
                ++paren_depth;
            } else if (c == ')') {
                paren_depth = paren_depth > 0 ? paren_depth - 1 : 0;
            }
        }

        if (c == ',' && bracket_depth == 0 && paren_depth == 0 && !in_single && !in_double) {
            std::string trimmed = trim_copy(current);
            if (!trimmed.empty()) {
                out.push_back(trimmed);
            }
            current.clear();
        } else {
            current.push_back(c);
        }
    }

    std::string trimmed = trim_copy(current);
    if (!trimmed.empty()) {
        out.push_back(trimmed);
    }
    return out;
}

} // namespace

ParsedLine parse_line(const std::string& line)
{
    ParsedLine result;

    std::string without_comment = strip_comment(line);
    std::string trimmed = trim_copy(without_comment);

    if (trimmed.empty()) {
        result.kind = LineKind::Empty;
        return result;
    }

    size_t first_space = trimmed.find_first_of(" \t");
    size_t colon_pos = trimmed.find(':');
    if (colon_pos != std::string::npos &&
        (first_space == std::string::npos || colon_pos < first_space)) {
        result.label = trimmed.substr(0, colon_pos);
        if (!parser_asm(result.label.c_str())) {
            result.kind = LineKind::Error;
            result.error = "Invalid label: " + result.label;
            return result;
        }
        trimmed = trim_copy(trimmed.substr(colon_pos + 1));
        if (trimmed.empty()) {
            result.kind = LineKind::Label;
            return result;
        }
    }

    size_t opcode_end = trimmed.find_first_of(" \t");
    if (opcode_end == std::string::npos) {
        result.opcode = trimmed;
        trimmed.clear();
    } else {
        result.opcode = trimmed.substr(0, opcode_end);
        trimmed = trim_copy(trimmed.substr(opcode_end + 1));
    }

    if (!parser_asm(result.opcode.c_str())) {
        result.kind = LineKind::Error;
        result.error = "Invalid opcode: " + result.opcode;
        return result;
    }

    if (!trimmed.empty()) {
        result.operands = split_operands(trimmed);
    }

    if (is_directive_token(result.opcode)) {
        result.kind = LineKind::Directive;
    } else if (is_instruction_token(result.opcode)) {
        result.kind = LineKind::Instruction;
    } else {
        result.kind = LineKind::Error;
        result.error = "Unknown opcode: " + result.opcode;
    }

    return result;
}

} // namespace uasm
