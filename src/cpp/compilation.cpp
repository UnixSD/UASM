#include "uasm/cpp/compilation.h"

__USED_API__ void __init_llvm() noexcept 
{
    InitializeAllTargetInfos();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();
}

__USED_API__ Target* __get_target(const string& triple_str) 
{
    string error;
    Triple triple(Triple::normalize(triple_str));
    
    const Target* target = TargetRegistry::lookupTarget(
        triple.getTriple(), error);
    
    return const_cast<Target*>(target);
}

__USED_API__ MCContext* __build_mc_context(Triple& triple) 
{
    MCTargetOptions options;
    MCAsmInfo* asm_info = nullptr;
    
    return new MCContext(triple, asm_info, nullptr, nullptr, options);
}

__USED_API__ llvm::Error __parse_asm(const string& asm_code, MCContext* ctx)
{
    if (!ctx) {
        return createStringError(inconvertibleErrorCode(), "MCContext is null");
    }
    if (asm_code.empty()) {
        return createStringError(inconvertibleErrorCode(), "asm_code is empty");
    }
    return llvm::Error::success();
}

__USED_API__ void __read_obj(const string& filename) 
{
    auto binary_or_error = object::createBinary(filename);
    if (!binary_or_error) {
        errs() << "Error reading file: " << toString(binary_or_error.takeError()) << "\n";
        return;
    }
    auto& binary = binary_or_error.get();
    if (auto* obj = dyn_cast<object::ObjectFile>(binary.getBinary())) 
    {
        outs() << "Object file: " << filename << "\n";
        for (auto& section : obj->sections()) 
        {
            Expected<StringRef> name = section.getName();
            if (name) 
            {
                outs() << "Section: " << *name << "\n";
            }
        }
    }
}

// void example_output() {
//     // Стандартный вывод
//     outs() << "Output to stdout\n";
//     errs() << "Output to stderr\n";
    
//     // Вывод в строку
//     std::string str;
//     raw_string_ostream os(str);
//     os << "Output to string: " << 42 << "\n";
//     outs() << "String content: " << os.str();
// }

// // Пример 7: Работа с тройками архитектур
// void example_triple() {
//     Triple triple("x86_64-unknown-linux-gnu");
    
//     outs() << "Architecture: " << triple.getArchName() << "\n";
//     outs() << "Vendor: " << triple.getVendorName() << "\n";
//     outs() << "OS: " << triple.getOSName() << "\n";
//     outs() << "Environment: " << triple.getEnvironmentName() << "\n";
    
//     // Проверка архитектуры
//     if (triple.isX86()) {
//         outs() << "This is x86 architecture\n";
//     }
    
//     if (triple.isArch64Bit()) {
//         outs() << "This is 64-bit architecture\n";
//     }
// }

// // Пример 8: Обработка ошибок LLVM
// void example_error_handling() {
//     // Создание ошибки
//     auto error = createStringError(
//         inconvertibleErrorCode(),
//         "Custom error message");
    
//     // Проверка ошибки
//     if (error) {
//         errs() << "Error: " << toString(std::move(error)) << "\n";
//     }
    
//     // Или с использованием logAllUnhandledErrors
//     logAllUnhandledErrors(std::move(error), errs(), "Error: ");
// }
