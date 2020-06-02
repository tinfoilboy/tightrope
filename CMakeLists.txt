cmake_minimum_required (VERSION 3.8)

project ("tightrope" VERSION 1.0.0 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable (tightrope source/main.cpp)

if(MSVC)
  target_compile_options (tightrope PRIVATE /W4 /WX)
else()
  target_compile_options (tightrope PRIVATE -Wall -Wextra -pedantic -Werror)
endif()