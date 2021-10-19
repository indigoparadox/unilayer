
cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

project(unilayer)

set(UNILAYER_C_FILES
   dio.c
   graphics.c)

# === Config ===

if(NOT DEFINED BACKEND)
   set(BACKEND sdl)
endif()

# === Engine Definitions ===

list(APPEND UNILAYER_C_FILES
   graphics/${BACKEND}g.c
   input/${BACKEND}i.c)

if(WIN32)
list(APPEND UNILAYER_C_FILES memory/winm.c)
else()
list(APPEND UNILAYER_C_FILES memory/fakem.c)
endif()

# === Library ===

add_library(unilayer ${UNILAYER_C_FILES})
target_compile_definitions(unilayer PUBLIC ${UNILAYER_DEFINES})

if(BACKEND MATCHES "sdl")
    if(MINGW)
        pkg_check_modules(SDL REQUIRED sdl)
    else()
        find_package(SDL REQUIRED)
    endif()
    target_include_directories(unilayer PUBLIC ${SDL_INCLUDE_DIRS})
endif()

