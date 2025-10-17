#
#   Unit Tests
#
include(Testing)

set(SOURCES
    Source/Mesh_utest.cpp
    Source/MeshCirculator_utest.cpp
    Source/MeshExporter_utest.cpp
    Source/MeshIntegrity_utest.cpp
    Source/MeshLoader_utest.cpp
    Source/Primitive_utest.cpp
    Source/PrimitiveProxy_utest.cpp
    Source/VertexPair_utest.cpp
)

add_executable(AppUnitTests)

target_sources(AppUnitTests PRIVATE ${SOURCES})

target_include_directories(AppUnitTests PRIVATE Include)

target_link_libraries(AppUnitTests PRIVATE AppStaticLib warning_properties)

# Copy Data files into build tree 
add_custom_command(TARGET AppUnitTests POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    "${CMAKE_SOURCE_DIR}/App/Test/TestFiles" 
    $<TARGET_FILE_DIR:AppUnitTests>/TestFiles)

AddTests(AppUnitTests)
AddCppCheck(AppUnitTests)
