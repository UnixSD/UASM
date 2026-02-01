#pragma once

#include <string>
#include <vector>

namespace uasm {

enum class LineKind {
    Empty,
    Comment,
    Label,
    Directive,
    Instruction,
    Error
};

struct ParsedLine {
    LineKind kind = LineKind::Empty;
    std::string label;
    std::string opcode;
    std::vector<std::string> operands;
    std::string error;
};

ParsedLine parse_line(const std::string& line);

} // namespace uasm
