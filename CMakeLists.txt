cmake_minimum_required(VERSION 3.12)
project(KrisRaycaster)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TARGET_PLATFORM "DESKTOP" CACHE STRING "DESKTOP")
set_property(CACHE TARGET_PLATFORM PROPERTY STRINGS DESKTOP)

# Common source files
#set(COMMON_SOURCES
#        src/${PROJECT_NAME.c
#        src/map.c
#        src/player.c
#)

include(CheckCXXCompilerFlag)
function(enable_cxx_compiler_flag_if_supported flag)
    string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
    if (flag_already_set EQUAL -1)
        check_cxx_compiler_flag("${flag}" flag_supported)
        if (flag_supported)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
        endif ()
        unset(flag_supported CACHE)
    endif ()
endfunction()

file(GLOB SOURCES
        "${SRC_DIR}/*.h"
        "${SRC_DIR}/*.cpp"
)

enable_cxx_compiler_flag_if_supported("-Wall")
enable_cxx_compiler_flag_if_supported("-Wextra")
enable_cxx_compiler_flag_if_supported("-pedantic")
enable_cxx_compiler_flag_if_supported("-std=c++17")
enable_cxx_compiler_flag_if_supported("-O3")
add_compile_definitions(_USE_MATH_DEFINES)

if (TARGET_PLATFORM STREQUAL "DESKTOP")
    add_executable(${PROJECT_NAME} ${SOURCES})
    add_executable(test_raycast src/debug/test_raycast/test_raycast.cpp)
    set_target_properties(${PROJECT_NAME}
    PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR})
    find_package(SDL2 REQUIRED)
    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
            $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_DESKTOP USE_SDL)
    find_package(SDL2_image CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)
    find_package(SDL2_ttf CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>)
else ()
    message(FATAL_ERROR "Unsupported TARGET_PLATFORM: ${TARGET_PLATFORM}")
endif ()

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

