cmake_minimum_required(VERSION 3.20)

project(MiniPyPlusPlus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find all implementation files
file(GLOB_RECURSE MINIPY_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/AST/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Builtins/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Lexer/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Parser/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Semantic/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Value.cpp"
)

# Core interpreter code
add_library(minipy_core ${MINIPY_SOURCES})

target_include_directories(minipy_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if (MSVC)
    target_compile_options(minipy_core PRIVATE /W4)
else()
    target_compile_options(minipy_core PRIVATE -Wall -Wextra)
endif()

# Main interpreter
add_executable(minipy main.cpp)

target_link_libraries(minipy
    PRIVATE minipy_core
)

# Tests
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Tests/tests.cpp")
        file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/Tests/*.cpp"
    )

    add_executable(minipy_tests
        ${TEST_SOURCES}
    )

    target_link_libraries(minipy_tests
        PRIVATE minipy_core
    )

    enable_testing()

    add_test(
        NAME MiniPyTests
        COMMAND minipy_tests
    )
endif()