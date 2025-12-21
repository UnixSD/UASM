#include "object.h"

__USED_API__ object::ObjectFile* __load_obj(const string& filename) const
{
    auto binary_or_error = object::createBinary(filename);
    if (!binary_or_error) 
    {
        errs() << "Error reading file: " << toString(binary_or_error.takeError()) << "\n";
        return nullptr;
    }
    return binary_or_error.get();
}

