#
#   Unit Tests
#

include(Testing)

set(SOURCES
    Source/MeshLoader_utest.cpp
)

add_executable(AppUnitTests)

target_sources(AppUnitTests PRIVATE ${SOURCES})

target_link_libraries(AppUnitTests PRIVATE AppStaticLib)

# Copy Data files into build tree 
add_custom_command(TARGET AppUnitTests POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    "${CMAKE_SOURCE_DIR}/App/Test/TestFiles" 
    $<TARGET_FILE_DIR:AppUnitTests>/TestFiles)

AddTests(AppUnitTests)
