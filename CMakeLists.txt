cmake_minimum_required(VERSION 3.10)
project(UASM VERSION 2.56 LANGUAGES C)

# Настройка стандарта C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Опции проекта
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(ENABLE_DEBUG "Enable debug build" OFF)

# Определяем платформу
if(UNIX AND NOT APPLE)
    set(PLATFORM_LINUX TRUE)
    add_definitions(-D__UNIX__)
endif()

# Директории с исходниками
set(SRC_DIR ${CMAKE_SOURCE_DIR})
set(HEADER_DIR ${CMAKE_SOURCE_DIR}/H)

# Включаем директорию с заголовочными файлами
include_directories(${HEADER_DIR})

# Список исходных файлов
set(UASM_SOURCES
    lib/assembly.h
    ${SRC_DIR}/main.cpp
    lib/preproc.h
    lib/compiler.h
    lib/linker.h
    lib/lexer.h
    lib/linker.ixx
    lib/preproc.h
    lib/reader.h
    lib/syntax.h

    ${SRC_DIR}/assembly.cpp
    ${SRC_DIR}/preproc.cpp
}

# Определяем флаги компилятора
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=20")
    if(ENABLE_DEBUG)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -DDEBUG")
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -DNDEBUG")
    endif()

# Создаём исполняемый файл
add_executable(uasm ${UASM_SOURCES})

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
