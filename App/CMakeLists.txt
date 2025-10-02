#
# App
#

set(SOURCES
    Source/Main.cpp
    Source/AppLayer.cpp
)

add_executable(App)

target_sources(App PRIVATE ${SOURCES})

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

add_compile_definitions(
    GLFW_INCLUDE_NONE
)

target_link_libraries(App Core)

target_include_directories(App PRIVATE Include)

# Set working directory for VS debugger
set_target_properties(App PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/App"
)

# Copy shader files into build tree 
add_custom_command(TARGET App POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    "${CMAKE_SOURCE_DIR}/App/Shaders" 
    $<TARGET_FILE_DIR:App>/Shaders)


# Unit tests
include(Testing)
add_subdirectory(Test)
 