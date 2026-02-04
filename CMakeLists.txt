cmake_minimum_required(VERSION 3.10)
project(UASM VERSION 2.56 LANGUAGES C CXX)

# Настройка стандарта C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Опции проекта
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(ENABLE_DEBUG "Enable debug build" OFF)
set(UASM_TARGET_BITS "64" CACHE STRING "Target bit width (16/32/64)")
set_property(CACHE UASM_TARGET_BITS PROPERTY STRINGS 16 32 64)

# Определяем платформу
if(UNIX AND NOT APPLE)
    set(PLATFORM_LINUX TRUE)
    add_definitions(-D__UNIX__)
endif()

# Директории с исходниками
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(HEADER_DIR ${CMAKE_SOURCE_DIR}/include)
set(MODULE_DIR ${CMAKE_SOURCE_DIR}/modules)

# Включаем директорию с заголовочными файлами
include_directories(${HEADER_DIR})

# Список исходных файлов
set(UASM_SOURCES
    ${SRC_DIR}/cpp/main.cpp
    ${SRC_DIR}/cpp/assembly.cpp
    ${SRC_DIR}/cpp/parser.cpp
    ${SRC_DIR}/cpp/syntax.cpp
    ${SRC_DIR}/c/preproc.c
    ${SRC_DIR}/c/lexer.c
    ${SRC_DIR}/c/parser.c

    ${HEADER_DIR}/uasm/c/asm.h
    ${HEADER_DIR}/uasm/c/lexer.h
    ${HEADER_DIR}/uasm/c/preproc.h
    ${HEADER_DIR}/uasm/cpp/assembly.h
    ${HEADER_DIR}/uasm/cpp/compilation.h
    ${HEADER_DIR}/uasm/cpp/compiler.h
    ${HEADER_DIR}/uasm/cpp/lexer.h
    ${HEADER_DIR}/uasm/cpp/linker.h
    ${HEADER_DIR}/uasm/cpp/object.h
    ${HEADER_DIR}/uasm/cpp/preproc.h
    ${HEADER_DIR}/uasm/cpp/reader.h
    ${HEADER_DIR}/uasm/cpp/syntax.h

    ${MODULE_DIR}/linker.ixx
)

# Определяем флаги компилятора
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
    if(ENABLE_DEBUG)
        add_compile_options(-g)
        add_definitions(-DDEBUG)
    else()
        add_compile_options(-O2)
        add_definitions(-DNDEBUG)
    endif()
endif()

# Создаём исполняемый файл
add_executable(uasm ${UASM_SOURCES})

if(UASM_TARGET_BITS STREQUAL "16")
    set(UASM_ARCH_DEF _UASM16_)
elseif(UASM_TARGET_BITS STREQUAL "32")
    set(UASM_ARCH_DEF _UASM32_)
elseif(UASM_TARGET_BITS STREQUAL "64")
    set(UASM_ARCH_DEF _UASM64_)
else()
    message(FATAL_ERROR "UASM_TARGET_BITS must be 16, 32, or 64")
endif()

target_compile_definitions(uasm PRIVATE ${UASM_ARCH_DEF})

# Линковка с библиотеками
if(UNIX)
    target_link_libraries(uasm m)
endif()

# Установка
install(TARGETS uasm DESTINATION bin)

# Опциональная установка документации и примеров
if(EXISTS ${CMAKE_SOURCE_DIR}/Readme.txt)
    install(FILES ${CMAKE_SOURCE_DIR}/Readme.txt DESTINATION share/doc/uasm)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/History.txt)
    install(FILES ${CMAKE_SOURCE_DIR}/History.txt DESTINATION share/doc/uasm)
endif()

# Тестирование (если есть тесты)
enable_testing()
if(EXISTS ${CMAKE_SOURCE_DIR}/test)
    add_subdirectory(test)
endif()
