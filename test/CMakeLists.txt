add_executable(uasm_tests
    test_main.cpp
    ${PROJECT_SOURCE_DIR}/src/c/preproc.c
    ${PROJECT_SOURCE_DIR}/src/c/lexer.c
    ${PROJECT_SOURCE_DIR}/src/c/parser.c
    ${PROJECT_SOURCE_DIR}/src/cpp/parser.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/syntax.cpp
    ${PROJECT_SOURCE_DIR}/src/cpp/assembly.cpp
)

target_include_directories(uasm_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_features(uasm_tests PRIVATE cxx_std_20)
if(DEFINED UASM_ARCH_DEF)
    target_compile_definitions(uasm_tests PRIVATE ${UASM_ARCH_DEF})
endif()

add_test(NAME uasm_tests COMMAND uasm_tests)
