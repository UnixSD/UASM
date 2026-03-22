# UASM
UASM (Unix Assembly) is an LLVM compiler written in C and C++ (without using Assembly).

# Build
The compiler is under active development. Use CMake with an explicit target bit width.

Example:
```
cmake -S . -B build -DUASM_TARGET_BITS=64
cmake --build build
```

# CLI options
- `-f 16|32|64` target bit width for LLVM backend
- `-o <file>` output file
- `-I <path>` add include path for the preprocessor
- `--internal` assemble using the internal encoder and emit `.hex` (skips LLVM)
- `--strict` treat unknown opcodes as errors
- `--no-warn-unknown` silence warnings for unknown opcodes

## Structure
- include/uasm/c: C headers (preprocessor, asm definitions)
- include/uasm/cpp: C++ headers (LLVM pipeline, linker, compiler)
- src/c: C implementation (preprocessor)
- src/cpp: C++ implementation (LLVM assembler/compiler)
- modules: C++20 modules
- docs: documentation
- examples: sample assembly inputs

![graph](https://github.com/UnixSD/UASM/blob/main/docs/graph.png)
