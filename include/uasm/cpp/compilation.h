#pragma once
#include "uasm/c/asm.h"
#include <string>
#define __USED_API__ __attribute__((used))

#ifdef __cplusplus
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Error.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/MC/MCContext.h>
#include <llvm/MC/MCStreamer.h>
#include <llvm/MC/MCAsmInfo.h>
#include <llvm/MC/MCInstrInfo.h>
#include <llvm/MC/MCRegisterInfo.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/Target.h>
#include <llvm/TargetParser/Triple.h>
#include <llvm/AsmParser/Parser.h>
#include <llvm/Object/ObjectFile.h>
#include <llvm/Object/SymbolicFile.h>
#include <llvm/BinaryFormat/ELF.h>
#include <llvm/BinaryFormat/MachO.h>
#include <llvm/Option/ArgList.h>
#include <llvm/Option/OptTable.h>

#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/MC/MCAsmBackend.h>
#include <llvm/MC/MCCodeEmitter.h>
#include <llvm/MC/MCSubtargetInfo.h>
using namespace std;
using namespace llvm;

__USED_API__ void __init_llvm() noexcept;
__USED_API__ Target* __get_target(const string& triple_str);
__USED_API__ MCContext* __build_mc_context(Triple& triple);
__USED_API__ llvm::Error __parse_asm(const string& asm_code, MCContext* ctx);
__USED_API__ void __read_obj(const string& filename);
#else
#error "LLVM is not supported"
#endif
