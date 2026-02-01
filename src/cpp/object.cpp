#include "uasm/cpp/object.h"

__USED_API__ llvm::Expected<llvm::object::OwningBinary<llvm::object::Binary>>
__load_obj(const std::string& filename)
{
    return llvm::object::createBinary(filename);
}
