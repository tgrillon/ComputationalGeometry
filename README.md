# MeshToolbox

## Overview

This project aims to provide various algorithmic tools for meshes, as well as other features related to 3D object modeling. Eventually, an interactive graphical interface will allow visualization of the results from different algorithms.

## Development Guidelines

This project follows strict development practices to ensure code quality, reliability, and maintainability.

### Code Quality Standards

- **Unit Testing**: Every feature must include comprehensive unit tests. All new code should be accompanied by tests that verify its functionality and edge cases.
- **Code Coverage**: Maintain high code coverage standards (>90%).
- **Memory Safety**: All code must pass memory leak detection. Use Valgrind/Memcheck to verify proper memory management.
- **Static Analysis**: Code must pass static analysis checks.
- **Code Formatting**: Follow the project's coding style. Use the provided formatting tools to ensure consistency.

### Continuous Integration

All changes are automatically validated through the CI pipeline, which runs:
- Unit tests in Debug and Release
- Code coverage analysis
- Memory leak detection
- Static code analysis
- Documentation generation

## Key Features

- **Mesh Data structure** : Face-Vertex representation. Exta data such as normal or texture coordinates can be added to each primitive of the mesh.  
- **Load/Export Mesh** : Load and export mesh into two formats (off and obj).
- **Display interactive window** : Configurable interface (ImGui). 

### Planned features

- **3D Viewer** : Implementation of an orbiter-type camera to visualize meshes in 2D (orthographic)/3D space. Quaternions will be used to avoid gimbal lock issues.
- **Computational Geometry Algorithms** : Implementation of several algorithms on meshes, such as curvature computation, remeshing, and geometrical modeling using bezier patches and curves.  

## Getting Started

### Prerequisites

- Env : Linux (Windows and MacOS not supported yet)
- Compiler: GCC C++20 or later
- CMake 3.15+
- Doxygen
- Valgrind
- CppCheck

### Installation & Build

```bash
git clone https://github.com/tgrillon/MeshToolBox.git
cd MeshToolBox
cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build/ -j 16
```
## Modules

The project is organized into the following modules:

- **Core Module**: Rendering + Print utilities. 
- **Application Module**: Mesh features. 

## License

This repository uses [The Unlicense](https://unlicense.org/), so feel free to use this however you like.

## Support

- **Documentation**: https://tgrillon.github.io/MeshToolBox/docs/index.html
- **Coverage report**: https://tgrillon.github.io/MeshToolBox/coverage/index.html
- **Memcheck report**: https://tgrillon.github.io/MeshToolBox/memcheck/index.html
- **Email**: theogrillon6f9@gmail.com

## Authors and Acknowledgments

- **Théo GRILLON** - [tgrillon](https://github.com/tgrillon)
- *Project structure inspired by [TheCherno/Architecture](https://github.com/TheCherno/Architecture)*
- CMake configuration based on this book : *Modern CMake for C++ by Rafal Świdziński*
