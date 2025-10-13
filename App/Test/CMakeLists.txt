#
#   Unit Tests
#
include(Testing)

set(SOURCES
    Source/Mesh_utest.cpp
    Source/MeshIntegrity_utest.cpp
    Source/MeshLoader_utest.cpp
    Source/Primitive_utest.cpp
    Source/PrimitiveProxy_utest.cpp
    Source/VertexPair_utest.cpp
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
