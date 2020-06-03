cmake_minimum_required (VERSION 3.16)

project ("test" VERSION 1.0.0 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory (extern/spdlog)

add_executable (tightrope source/pch.cpp source/tightrope.cpp source/window.cpp source/main.cpp)

target_include_directories (tightrope PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/extern/spdlog/include)

if(MSVC)
    target_compile_options (tightrope PRIVATE /W4 /WX)
else()
    target_compile_options (tightrope PRIVATE -Wall -Wextra -pedantic -Werror)

    # VS 2019 currently has a bug where "target_precompile_headers" cannot be used, currently disabled unless on another platform
    target_precompile_headers (tightrope PUBLIC ${CMAKE_SOURCE_DIR}/include/tightrope/pch.hpp)
endif()