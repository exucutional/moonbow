cmake_minimum_required(VERSION 3.19)

project(moonbow CXX)

set(SRC_FILES
    "src/main.cpp"
    "src/glfw_window.cpp"
)

set(HEADER_FILES
    "src/glfw_window.h"
)

set(SHADER_FILES
    ""
)

include_directories(src)
include_directories(external/glfw/include)

add_subdirectory(external/glfw)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES} ${SHADER_FILES})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
