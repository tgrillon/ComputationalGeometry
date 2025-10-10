#pragma once

#include "Application/Mesh.h"

#include <filesystem>

namespace Utilitary::Mesh
{
/// @brief Struct for loading meshes from files.
struct MeshLoader
{
	/// @brief Load mesh from an OFF file.
	/// @param filepath Path to the OFF file.
	/// @note This function assumes the file is in OFF format.
	/// @return Pointer to the loaded mesh, or nullptr if loading failed.
	static std::unique_ptr<Data::Surface::Mesh> LoadOFF(const std::filesystem::path& filepath);

	/// @brief Load mesh from an OBJ file.
	/// @param filepath Path to the OBJ file.
	/// @note This function assumes the file is in OBJ format.
	/// @return Pointer to the loaded mesh, or nullptr if loading failed.
	static std::unique_ptr<Data::Surface::Mesh> LoadOBJ(const std::filesystem::path& filepath);
};
} // namespace Utilitary::Mesh
