#
# App
#

set(SOURCES
    Source/RunApp.cpp
    Source/AppLayer.cpp
)

add_library(AppStaticLib STATIC ${SOURCES})

target_sources(AppStaticLib PRIVATE ${SOURCES})


# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

add_compile_definitions(
    GLFW_INCLUDE_NONE
)
    
target_link_libraries(AppStaticLib Core)
    
target_include_directories(AppStaticLib PUBLIC Include)
    
# Set working directory for VS debugger
set_target_properties(AppStaticLib PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/AppStaticLib"
)

include(BuildInfo)
BuildInfo(AppStaticLib)

InstrumentForCoverage(AppStaticLib)
CleanCoverage(AppStaticLib)

Format(AppStaticLib .)

# Unit tests
add_subdirectory(Test)

# Main executable
add_executable(App EntryPoint.cpp)
target_link_libraries(App PRIVATE AppStaticLib)

# Copy Data files into build tree 
add_custom_command(TARGET App POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    "${CMAKE_SOURCE_DIR}/App/Data" 
    $<TARGET_FILE_DIR:App>/Data)
