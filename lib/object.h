#include <llvm/Object/ObjectFile.h>
#include <llvm/Support/Error.h>

#define __USED_API__ __attribute__((used))
#ifdef __cplusplus
__USED_API__ object::ObjectFile* __load_obj(const string& filename) const;
#endif