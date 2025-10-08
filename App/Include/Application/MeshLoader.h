#pragma once

#include "Application/Mesh.h"

#include <filesystem>

namespace Utilitary
{
/// @brief Class for loading meshes from files.
class MeshLoader
{
public:
	/// @brief Load mesh from an OFF file.
	/// @param filepath Path to the OFF file.
	/// @return Pointer to the loaded mesh, or nullptr if loading failed.
	static std::unique_ptr<Data::Surface::Mesh> LoadOFF(const std::filesystem::path& filepath);

	/// @brief Load mesh from an OBJ file.
	/// @param filepath Path to the OBJ file.
	/// @return Pointer to the loaded mesh, or nullptr if loading failed.
	static std::unique_ptr<Data::Surface::Mesh> LoadOBJ(const std::filesystem::path& filepath);
};
} // namespace Utilitary
