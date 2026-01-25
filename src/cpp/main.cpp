#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>

// LLVM includes
#pragma cling add_library(llvm-19 "libLLVM-19.so")
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Triple.h>
#include <llvm/MC/MCAsmInfo.h>
#include <llvm/MC/MCContext.h>
#include <llvm/MC/MCDisassembler/MCDisassembler.h>
#include <llvm/MC/MCInstPrinter.h>
#include <llvm/MC/MCInstrInfo.h>
#include <llvm/MC/MCRegisterInfo.h>
#include <llvm/MC/MCSubtargetInfo.h>
#include <llvm/MC/MCStreamer.h>
#include <llvm/MC/MCAsmBackend.h>
#include <llvm/MC/MCCodeEmitter.h>
#include <llvm/MC/MCObjectFileInfo.h>
#include <llvm/MC/MCObjectStreamer.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/ToolOutputFile.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/MC/MCParser/MCAsmParser.h>
#include <llvm/MC/MCParser/MCTargetAsmParser.h>
#include <llvm/MC/MCAsmMacro.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/MC/MCObjectWriter.h>

// Preprocessor (C interface)
extern "C" {
#include "uasm/c/preproc.h"
}

using namespace llvm;
using namespace std;

// Глобальные переменные для опций компилятора
static int target_bit_width = 64;  // По умолчанию 64-бит
static string output_file;
static vector<string> include_paths;
static string input_file;

// Функция для парсинга аргументов командной строки
void parse_arguments(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            target_bit_width = atoi(argv[++i]);
            if (target_bit_width != 16 && target_bit_width != 32 && target_bit_width != 64) {
                cerr << "Error: Invalid bit width. Use 16, 32, or 64." << endl;
                exit(1);
            }
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        } else if (strcmp(argv[i], "-I") == 0 && i + 1 < argc) {
            include_paths.push_back(argv[++i]);
        } else if (argv[i][0] != '-') {
            input_file = argv[i];
        } else {
            cerr << "Unknown option: " << argv[i] << endl;
            exit(1);
        }
    }

    if (input_file.empty()) {
        cerr << "Usage: " << argv[0] << " [-f 16|32|64] [-o output_file] [-I include_path] input_file" << endl;
        exit(1);
    }

    if (output_file.empty()) {
        // Если не указан выходной файл, используем имя входного файла с расширением .o
        output_file = input_file;
        size_t pos = output_file.find_last_of('.');
        if (pos != string::npos) {
            output_file = output_file.substr(0, pos);
        }
        output_file += ".o";
    }
}

// Функция для получения triple в зависимости от разрядности
string get_triple(int bit_width) {
    switch (bit_width) {
        case 16:
            return "i386-unknown-linux-gnu";  // 16-bit x86
        case 32:
            return "i386-unknown-linux-gnu";  // 32-bit x86
        case 64:
            return "x86_64-unknown-linux-gnu";  // 64-bit x86
        default:
            return "x86_64-unknown-linux-gnu";
    }
}

// Функция для получения CPU в зависимости от разрядности
string get_cpu(int bit_width) {
    switch (bit_width) {
        case 16:
            return "i386";
        case 32:
            return "i386";
        case 64:
            return "x86-64";
        default:
            return "x86-64";
    }
}

// Функция для компиляции ассемблера в объектный файл
int compile_assembly(const string& asm_code, const string& output_path, int bit_width, const string& input_filename) {
    // Инициализация LLVM
    InitializeAllTargetInfos();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();
    
    string triple_str = get_triple(bit_width);
    Triple triple(Triple::normalize(triple_str));
    
    string error;
    const Target* target = TargetRegistry::lookupTarget("", triple, error);
    if (!target) {
        cerr << "Error: " << error << endl;
        return 1;
    }
    
    string cpu = get_cpu(bit_width);
    string features = "";
    
    // Создание TargetMachine
    TargetOptions options;
    options.MCOptions.ABIName = "";
    Reloc::Model reloc_model = Reloc::PIC_;
    CodeModel::Model code_model = CodeModel::Small;
    
    std::unique_ptr<TargetMachine> target_machine(target->createTargetMachine(
        triple.getTriple(), cpu, features, options, reloc_model, code_model, CodeGenOptLevel::Default));
    
    if (!target_machine) {
        cerr << "Error: Failed to create target machine" << endl;
        return 1;
    }
    
    // Создание объектного файла
    std::error_code ec;
    raw_fd_ostream dest(output_path, ec, sys::fs::OF_None);
    if (ec) {
        cerr << "Error: Cannot open output file: " << ec.message() << endl;
        return 1;
    }
    
    // Создание контекста MC
    const MCSubtargetInfo& STI = *target_machine->getMCSubtargetInfo();
    const MCRegisterInfo& MRI = *target_machine->getMCRegisterInfo();
    const MCAsmInfo& MAI = *target_machine->getMCAsmInfo();
    
    std::unique_ptr<MCContext> ctx(new MCContext(triple, &MAI, &MRI, &STI));
    
    // Создание объектного файла info
    std::unique_ptr<MCObjectFileInfo> obj_file_info(new MCObjectFileInfo());
    ctx->setObjectFileInfo(obj_file_info.get());
    obj_file_info->initMCObjectFileInfo(triple, options.MCOptions, reloc_model, code_model, *ctx);
    
    // Создание code emitter и asm backend
    const MCInstrInfo& MII = *target_machine->getMCInstrInfo();
    MCCodeEmitter* code_emitter = target->createMCCodeEmitter(MII, *ctx);
    if (!code_emitter) {
        cerr << "Error: Failed to create code emitter" << endl;
        return 1;
    }
    
    MCAsmBackend* asm_backend = target->createMCAsmBackend(STI, MRI, options.MCOptions);
    if (!asm_backend) {
        cerr << "Error: Failed to create asm backend" << endl;
        return 1;
    }
    
    // Создание object writer
    std::unique_ptr<MCObjectWriter> object_writer(asm_backend->createObjectWriter(dest));
    std::unique_ptr<MCStreamer> streamer(target->createMCObjectStreamer(
        triple, *ctx, std::unique_ptr<MCAsmBackend>(asm_backend),
        std::move(object_writer),
        std::unique_ptr<MCCodeEmitter>(code_emitter),
        STI));
    
    // Создание парсера ассемблера
    SourceMgr src_mgr;
    std::unique_ptr<MemoryBuffer> buffer = MemoryBuffer::getMemBufferCopy(asm_code, input_filename);
    src_mgr.AddNewSourceBuffer(std::move(buffer), SMLoc());
    
    std::unique_ptr<MCAsmParser> parser(createMCAsmParser(src_mgr, *ctx, *streamer, MAI));
    
    std::unique_ptr<MCTargetAsmParser> target_parser(
        target->createMCAsmParser(STI, *parser, *target_machine->getMCInstrInfo(), options.MCOptions));
    
    parser->setTargetParser(*target_parser);
    
    // Парсинг и генерация объектного файла
    if (parser->Run(false)) {
        cerr << "Error: Failed to parse assembly" << endl;
        return 1;
    }
    
    // Завершение генерации
    streamer->finish();
    dest.flush();
    
    return 0;
}

int main(int argc, char* argv[]) {
    // Парсинг аргументов
    parse_arguments(argc, argv);
    
    // Создание контекста препроцессора
    PreprocContext* preproc_ctx = preproc_create_context();
    if (!preproc_ctx) {
        cerr << "Error: Failed to create preprocessor context" << endl;
        return 1;
    }
    
    // Добавление путей для поиска include файлов
    preproc_add_include_path(preproc_ctx, ".");
    preproc_add_include_path(preproc_ctx, "/usr/include");
    for (const auto& path : include_paths) {
        preproc_add_include_path(preproc_ctx, path.c_str());
    }
    
    // Обработка препроцессором
    string temp_output = output_file + ".tmp";
    FILE* temp_file = fopen(temp_output.c_str(), "w");
    if (!temp_file) {
        cerr << "Error: Cannot create temporary file" << endl;
        preproc_destroy_context(preproc_ctx);
        return 1;
    }
    
    int preproc_result = preproc_process_file(preproc_ctx, input_file.c_str(), temp_file);
    fclose(temp_file);
    
    if (preproc_result != 0) {
        cerr << "Error: Preprocessing failed" << endl;
        preproc_destroy_context(preproc_ctx);
        remove(temp_output.c_str());
        return 1;
    }
    
    preproc_destroy_context(preproc_ctx);
    
    // Чтение обработанного файла
    ifstream processed_file(temp_output);
    if (!processed_file.is_open()) {
        cerr << "Error: Cannot open processed file" << endl;
        remove(temp_output.c_str());
        return 1;
    }
    
    stringstream buffer;
    buffer << processed_file.rdbuf();
    string asm_code = buffer.str();
    processed_file.close();
    
    // Удаление временного файла
    remove(temp_output.c_str());
    
    // Компиляция в объектный файл
    int compile_result = compile_assembly(asm_code, output_file, target_bit_width, input_file);
    
    if (compile_result == 0) {
        cout << "Successfully compiled to: " << output_file << endl;
    }
    
    return compile_result;
}
