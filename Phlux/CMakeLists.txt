#
# Phlux
#

set(SOURCES
    Source/Main.cpp
    Source/ViewportLayer.h
    Source/ViewportLayer.cpp
    Source/TitleBarLayer.h
    Source/TitleBarLayer.cpp
    Shaders/basicTexture.vert
    Shaders/basicTexture.frag
    Shaders/colorToPosition.vert
    Shaders/colorToPosition.frag
    Shaders/default.vert
    Shaders/default.frag
)

add_executable(Phlux)

target_sources(Phlux PRIVATE ${SOURCES})

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

add_compile_definitions(
    GLFW_INCLUDE_NONE
)

target_link_libraries(Phlux Core)

target_include_directories(Phlux PRIVATE Source)

# Set working directory for VS debugger
set_target_properties(Phlux PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Phlux"
)