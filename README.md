# UASM
UASM (Unix Assembly) is an LLVM compiler written in C and C++ (without using Assembly).

# Build 
The compiler is currently being tested and contains errors that prevent it from compiling.

## Structure
- include/uasm/c: C headers (preprocessor, asm definitions)
- include/uasm/cpp: C++ headers (LLVM pipeline, linker, compiler)
- src/c: C implementation (preprocessor)
- src/cpp: C++ implementation (LLVM assembler/compiler)
- modules: C++20 modules
- docs: documentation
- examples: sample assembly inputs
