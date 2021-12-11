
cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

project(unilayer)

# === Configuration ===

if(NOT DEFINED GRAPHICS)
   if(WIN32 OR MINGW)
      set(GRAPHICS win)
   elseif(UNIX)
      set(GRAPHICS sdl)
   endif()
endif()

if(NOT DEFINED MEMORY)
   if(WIN32 OR MINGW)
      set(MEMORY win)
   elseif(UNIX)
      set(MEMORY fake)
   endif()
endif()

if(NOT DEFINED RESOURCE)
   set(RESOURCE file)
endif()

# === Source Files ===

set(UNILAYER_SOURCE_FILES
   "src/dio.c"
   "src/animate.c"
   "src/graphics.c"
   "src/animate.c")

set(CONVERT_SOURCE_FILES
   "tools/convert.c"
   "tools/data/bmp.c"
   "tools/data/cga.c"
   "tools/data/icns.c"
   "src/dio.c"
   "src/memory/fakem.c")

set(UNILAYER_LIBRARIES)

set(UNILAYER_INCLUDES)

if(GRAPHICS MATCHES "sdl")
   list(APPEND UNILAYER_SOURCE_FILES
      "src/input/sdli.c"
      "src/graphics/sdlg.c")
   set(UNILAYER_DEFINES ${UNILAYER_DEFINES}
      -DPLATFORM_SDL
      -DUSE_SOFTWARE_TEXT)
   find_package(SDL2 REQUIRED)
   list(APPEND UNILAYER_LIBRARIES ${SDL2_LIBRARIES})
   list(APPEND UNILAYER_INCLUDES ${SDL2_INCLUDE_DIRS})
endif()

if(MEMORY MATCHES "fake")
   list(APPEND UNILAYER_SOURCE_FILES
      "src/memory/fakem.c")
endif()

if(RESOURCE MATCHES "file")
   list(APPEND UNILAYER_SOURCE_FILES
      "src/resource/file.c")
   set(UNILAYER_DEFINES ${UNILAYER_DEFINES} -DRESOURCE_FILE)
endif()

# === Assets ===

foreach(ASSET ${UNILAYER_ASSET_FILES})
   #message( ${ASSET} )
endforeach()

#add_custom_target("resext" ALL DEPENDS x)

# === Targets ===

add_executable(convert ${CONVERT_SOURCE_FILES})
target_compile_definitions(convert PUBLIC -DNO_RESEXT -DRESOURCE_FILE)

#add_custom_command(

message( ${UNILAYER_DEFINES} )
message( ${UNILAYER_INCLUDES} )

add_library(unilayer ${UNILAYER_SOURCE_FILES})
target_compile_definitions(unilayer PUBLIC ${UNILAYER_DEFINES})
target_include_directories(unilayer PUBLIC ${UNILAYER_INCLUDES})
target_link_libraries(unilayer PUBLIC ${UNILAYER_LIBRARIES})

#add_dependencies(unilayer resext)

