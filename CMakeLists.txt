cmake_minimum_required(VERSION 3.15)
project(SimpleCalculator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем FetchContent для автоматической загрузки зависимостей
include(FetchContent)

# Загрузка GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)

# Загрузка Dear ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.89.9
)

FetchContent_MakeAvailable(glfw imgui)

# Создаем библиотеку ImGui вручную (ImGui не имеет своего CMakeLists.txt)
add_library(imgui_lib STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_lib PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui_lib PUBLIC glfw)

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE imgui_lib glfw)

# Статическая линковка для Windows (MinGW)
if(WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE
        -static-libgcc
        -static-libstdc++
        -static
    )
endif()

# Для OpenGL на Windows
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
endif()