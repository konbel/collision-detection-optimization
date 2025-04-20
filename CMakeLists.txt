cmake_minimum_required(VERSION 3.30)
project(collision_detection_optimization)

find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)

set(CMAKE_CXX_STANDARD 20)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/vendor/imgui-1.91.9b)
set(IMGUI_BACKEND_DIR ${IMGUI_DIR}/backends)

add_executable(collision_detection_optimization src/Application.cpp
        src/rendering/Buffer.cpp
        src/rendering/VertexBuffer.cpp
        src/rendering/VertexArray.cpp
        src/rendering/IndexBuffer.cpp
        src/rendering/Shader.cpp
        src/rendering/Renderer.cpp

        src/engine/GameObject.cpp
        src/engine/Scene.cpp
        src/engine/PhysicsSolver.cpp

        src/TestScene.cpp

        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_BACKEND_DIR}/imgui_impl_glfw.cpp
        ${IMGUI_BACKEND_DIR}/imgui_impl_opengl3.cpp
)

include_directories(
        src/rendering
        src/vendor
        ${IMGUI_DIR}
        ${IMGUI_BACKEND_DIR}
)

target_link_libraries(collision_detection_optimization PRIVATE
        OpenGL::GL
        glfw
        GLEW::GLEW
)

add_custom_command(
        TARGET collision_detection_optimization POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        ${CMAKE_CURRENT_BINARY_DIR}/res
)