cmake_minimum_required(VERSION 3.12)
project(KrisRaycaster)

# set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(TARGET_PLATFORM "STM32" CACHE STRING "Target platform for build")
set_property(CACHE TARGET_PLATFORM PROPERTY STRINGS STM32 DESKTOP)

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

if (TARGET_PLATFORM STREQUAL "STM32")
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/stm32_toolchain.cmake)
    add_executable(${PROJECT_NAME} ${COMMON_SOURCES} src/main_stm32.c)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_STM32 STM32H750xx)
elseif (TARGET_PLATFORM STREQUAL "DESKTOP")
    find_package(SDL2 REQUIRED)
    add_executable(${PROJECT_NAME} ${SOURCES})
    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
            $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_DESKTOP USE_SDL)
else ()
    message(FATAL_ERROR "Unsupported TARGET_PLATFORM: ${TARGET_PLATFORM}")
endif ()

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
target_compile_definitions(${PROJECT_NAME} PRIVATE SCREEN_WIDTH=320 SCREEN_HEIGHT=240)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)