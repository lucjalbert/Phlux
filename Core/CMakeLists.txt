#
# Core
#

set(SOURCES
    Vendor/stb/stb_image.h
    Vendor/stb/stb_image.cpp
    Source/Core/Application.h
    Source/Core/Application.cpp
    Source/Core/Window.h
    Source/Core/Window.cpp
    Source/Core/Layer.h
    Source/Core/Layer.cpp
    Source/Core/Renderer/Shader.h
    Source/Core/Renderer/Shader.cpp
    Source/Core/Renderer/Texture2D.h
    Source/Core/Renderer/Texture2D.cpp
    Source/Core/Input/Input.h
    Source/Core/Input/Input.cpp
    Source/Core/Camera/FlyCamera.h
    Source/Core/Camera/FlyCamera.cpp
    Source/Core/ImGui/ImGuiLayer.h
    Source/Core/ImGui/ImGuiLayer.cpp
)

add_library(Core STATIC)

target_sources(Core PRIVATE ${SOURCES} ${IMGUI_CORE_SOURCES} ${IMGUI_BACKEND_SOURCES})

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

add_compile_definitions(
    GLFW_INCLUDE_NONE
)

target_link_libraries(Core 
    PUBLIC    
        glfw
        glad
        glm
    PRIVATE
        OpenGL::GL
)

target_include_directories(Core PUBLIC "Source" "vendor/stb" "${imgui_SOURCE_DIR}" PRIVATE "${imgui_SOURCE_DIR}/backends")