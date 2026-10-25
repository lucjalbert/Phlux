#
# Core
#

set(SOURCES
    Vendor/stb/stb_image.h
    Vendor/stb/stb_image.cpp
    Vendor/imfilebrowser/imfilebrowser.h

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
    Source/Core/Renderer/Mesh.h
    Source/Core/Renderer/Mesh.cpp
    Source/Core/Renderer/Model.h
    Source/Core/Renderer/Model.cpp

    Source/Core/Input/Input.h
    Source/Core/Input/Input.cpp

    Source/Core/Camera/FlyCamera.h
    Source/Core/Camera/FlyCamera.cpp

    Source/Core/ImGui/ImGuiLayer.h
    Source/Core/ImGui/ImGuiLayer.cpp

    Source/Core/Scene/Scene.h
    Source/Core/Scene/Scene.cpp
    Source/Core/Scene/Actors/Actor.h
    Source/Core/Scene/Actors/Actor.cpp
    Source/Core/Scene/Actors/ModelActor.h
    Source/Core/Scene/Actors/ModelActor.cpp
)

add_library(Core STATIC)

target_sources(Core PRIVATE ${SOURCES} ${IMGUI_CORE_SOURCES} ${IMGUI_BACKEND_SOURCES} ${ASSIMP_CORE_SOURCES})

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
        assimp
    PRIVATE
        OpenGL::GL
)

target_include_directories(Core PUBLIC "Source" "vendor/stb" "Vendor/imfilebrowser" "${imgui_SOURCE_DIR}" "${assimp_SOURCE_DIR}" PRIVATE "${imgui_SOURCE_DIR}/backends")