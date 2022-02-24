cmake_minimum_required(VERSION 3.10)
project(App)

set(CMAKE_CXX_STANDART 17)

#find_package(OpenGL REQUIRED)
#find_package(GLEW REQUIRED)
#find_package(glfw3 CONFIG REQUIRED)

file(GLOB SOURCES
    "src/*.cpp"
)
    
add_executable(Breakout ${SOURCES})
target_compile_features(Breakout PRIVATE cxx_std_17)
target_link_libraries(Breakout PUBLIC Fly)

add_custom_command(
        TARGET Breakout POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/App/data
                ${CMAKE_CURRENT_BINARY_DIR}/Debug)

add_custom_command(TARGET Breakout POST_BUILD        
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_BINARY_DIR}/../Engine/Debug   
                ${CMAKE_CURRENT_BINARY_DIR}/Debug)  

add_custom_command(
        TARGET Breakout POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/App/data
                ${CMAKE_CURRENT_BINARY_DIR}/Release)
            
add_custom_command(TARGET Breakout POST_BUILD        
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_BINARY_DIR}/../Engine/Release   
                ${CMAKE_CURRENT_BINARY_DIR}/Release)  
