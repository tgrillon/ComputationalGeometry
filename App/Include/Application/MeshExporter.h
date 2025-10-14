#pragma once

#include "Application/BaseType.h"
#include "Application/Mesh.h"

#include <cstdint>
#include <filesystem>

namespace Utilitary::Surface
{
/// @brief Struct for exporting meshes to different file formats.
struct MeshExporter
{
	/// @brief Export mesh to an OFF file.
	/// @param mesh Mesh to export.
	/// @param filename Name of the file to which the mesh is exported.
	/// @note This function assumes the mesh has a valid integrity.
	static void ExportOFF(const Data::Surface::Mesh& mesh, const std::filesystem::path& filepath);

	/// @brief Export mesh to an OBJ file.
	/// @param mesh Mesh to export.
	/// @param filename Name of the file to which the mesh is exported.
	/// @note This function assumes the mesh has a valid integrity.
	static void ExportOBJ(const Data::Surface::Mesh& mesh, const std::filesystem::path& filepath);
};
} // namespace Utilitary::Surface
