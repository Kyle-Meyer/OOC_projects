cmake_minimum_required(VERSION 3.15)
project(Poker CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Dear ImGui (fetched at configure time)
# ---------------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.91.6
)

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        master
)

FetchContent_MakeAvailable(imgui stb)

# imgui has no CMakeLists.txt — define the target manually
find_package(OpenGL REQUIRED)
find_package(glfw3  REQUIRED)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui PUBLIC OpenGL::GL glfw)

# ---------------------------------------------------------------------------
# Poker executable
# ---------------------------------------------------------------------------
file(GLOB SRC src/*.cpp)

add_executable(poker ${SRC})
target_include_directories(poker PRIVATE inc ${stb_SOURCE_DIR})
target_link_libraries(poker PRIVATE imgui)

# Copy card images next to the built executable so "cards/" resolves at runtime
add_custom_command(TARGET poker POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/cards $<TARGET_FILE_DIR:poker>/cards
    COMMENT "Copying card images to build output"
)
