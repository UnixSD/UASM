#pragma once

#include <string>

#include <llvm/Object/Binary.h>
#include <llvm/Object/ObjectFile.h>
#include <llvm/Support/Error.h>

#define __USED_API__ __attribute__((used))
#ifdef __cplusplus
__USED_API__ llvm::Expected<llvm::object::OwningBinary<llvm::object::Binary>>
__load_obj(const std::string& filename);
#endif
