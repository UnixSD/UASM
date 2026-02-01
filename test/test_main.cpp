#include "uasm/c/parser.h"
#include "uasm/c/preproc.h"
#include "uasm/cpp/assembly.h"
#include "uasm/cpp/parser.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

static int g_failures = 0;

static void expect_true(bool value, const char* message)
{
    if (!value) {
        std::fprintf(stderr, "FAIL: %s\n", message);
        ++g_failures;
    }
}

static void expect_eq(const std::string& actual, const std::string& expected, const char* message)
{
    if (actual != expected) {
        std::fprintf(stderr, "FAIL: %s (expected '%s', got '%s')\n",
                     message, expected.c_str(), actual.c_str());
        ++g_failures;
    }
}

static void expect_eq_int(int actual, int expected, const char* message)
{
    if (actual != expected) {
        std::fprintf(stderr, "FAIL: %s (expected %d, got %d)\n", message, expected, actual);
        ++g_failures;
    }
}

static std::string run_preproc(const char* input)
{
    PreprocContext* ctx = preproc_create_context();
    if (!ctx) {
        return "";
    }

    FILE* in = tmpfile();
    FILE* out = tmpfile();
    if (!in || !out) {
        if (in) std::fclose(in);
        if (out) std::fclose(out);
        preproc_destroy_context(ctx);
        return "";
    }

    std::fputs(input, in);
    std::rewind(in);

    preproc_process_stream(ctx, in, "test.asm", out);
    std::fflush(out);
    std::rewind(out);

    std::string result;
    char buffer[256];
    while (std::fgets(buffer, sizeof(buffer), out)) {
        result += buffer;
    }

    std::fclose(in);
    std::fclose(out);
    preproc_destroy_context(ctx);

    return result;
}

int main()
{
    expect_true(parser_asm("mov"), "parser_asm accepts valid token");
    expect_true(!parser_asm("mov$"), "parser_asm rejects invalid token");

    auto parsed = uasm::parse_line(".start: mov eax, ebx");
    expect_eq_int(static_cast<int>(parsed.kind),
                  static_cast<int>(uasm::LineKind::Instruction),
                  "parse_line detects instruction");
    expect_eq(parsed.label, ".start", "parse_line extracts label");
    expect_eq(parsed.opcode, "mov", "parse_line extracts opcode");
    expect_eq_int(static_cast<int>(parsed.operands.size()), 2, "parse_line extracts operands");
    if (parsed.operands.size() == 2) {
        expect_eq(parsed.operands[0], "eax", "parse_line operand 1");
        expect_eq(parsed.operands[1], "ebx", "parse_line operand 2");
    }

    auto directive = uasm::parse_line("%define FOO 1");
    expect_eq_int(static_cast<int>(directive.kind),
                  static_cast<int>(uasm::LineKind::Directive),
                  "parse_line detects directive");

    auto unknown = uasm::parse_line("foobar");
    expect_eq_int(static_cast<int>(unknown.kind),
                  static_cast<int>(uasm::LineKind::Error),
                  "parse_line detects unknown opcode");

    expect_eq(assemble_line("mov eax, ebx"), "89D8", "assemble_line MOV");
    expect_eq(assemble_line("add eax, 1"), "0500000001", "assemble_line ADD");

    std::string preproc_out = run_preproc("%define FOO 123\nFOO\n");
    expect_true(preproc_out.find("123") != std::string::npos, "preprocessor expands macro");

    if (g_failures == 0) {
        std::printf("All tests passed.\n");
    }

    return g_failures == 0 ? 0 : 1;
}
