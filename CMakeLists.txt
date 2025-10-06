cmake_minimum_required(VERSION 3.10)

project(Architecture)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Avoid in source build
include(NoInSourceBuilds)

# Fetch dependencies
include(Dependencies)

# Preset configuration
include(ConfigPresets)

include(Coverage)
include(Format)
include(CppCheck)
include(Memcheck)
include(Doxygen)

# Projects
add_subdirectory(Core)
add_subdirectory(App)
