cmake_minimum_required(VERSION 3.10)
project(shtrudel_program C)

# Set C standard
set(CMAKE_C_STANDARD 17)

# Add all source files
set(SOURCES
    shtrudel.c
)

# Add common warning flags and treat warnings as errors
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# Create executable
add_executable(shtrudel_program shtrudel.c)
