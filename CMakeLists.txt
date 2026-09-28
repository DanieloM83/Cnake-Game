cmake_minimum_required(VERSION 3.25)

project(cnake LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "Build raylib examples" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

FetchContent_Declare(
    raylib
    SYSTEM
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG dbc56a87da87d973a9c5baa4e7438a9d20121d28
)

FetchContent_MakeAvailable(raylib)

file(GLOB APP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)
file(GLOB APP_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

if(NOT APP_SOURCES)
    message(FATAL_ERROR "No C source files found in src/")
endif()

add_executable(cnake
    ${APP_SOURCES}
    ${APP_HEADERS}
)

target_include_directories(cnake
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(cnake
    PRIVATE
        raylib
)

if(MSVC)
    target_compile_options(cnake PRIVATE /W4)
else()
    target_compile_options(cnake PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

add_custom_target(run
    COMMAND $<TARGET_FILE:cnake>
    DEPENDS cnake
    WORKING_DIRECTORY $<TARGET_FILE_DIR:cnake>
    USES_TERMINAL
)