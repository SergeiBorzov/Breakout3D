cmake_minimum_required(VERSION 3.10)
project(Engine)

set(CMAKE_CXX_STANDART 17)

# TODO: deal with vcpkg path more nicely
include(C:\\Tools\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake)

# TODO: Specify debug and release build compiler options for different platforms

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

file(GLOB SOURCES
    "src/*.cpp"
    "src/core/*.cpp"
    "src/window/*.cpp"
    "src/math/*.cpp"
    "src/renderer/*.cpp"
    "src/physics/*.cpp"
    "src/scene/*.cpp"
    "src/script/*.cpp"
    "src/resource/*.cpp"
    "src/gui/*.cpp"
    "src/third_party/*.cpp"
)

file(GLOB HEADERS
    "src/*.h"
    "src/core/*.h"
    "src/window/*.h"
    "src/math/*.h"
    "src/renderer/*.h"
    "src/physics/*.h"
    "src/scene/*.h"
    "src/script/*.h"
    "src/resource/*.h"

    )

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DFLY_DEBUG_BUILD")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(Fly SHARED ${SOURCES})
target_include_directories(Fly PUBLIC src)
target_compile_features(Fly PRIVATE cxx_std_17)
target_compile_definitions(Fly PRIVATE FLY_COMPILING_DLL)
target_link_libraries(Fly PRIVATE ${OPENGL_LIBRARIES} GLEW::GLEW glfw)
