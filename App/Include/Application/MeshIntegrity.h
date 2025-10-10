#pragma once

#include "Application/Mesh.h"

#include <cstdint>

namespace Utilitary::Mesh
{
/// @brief Struct to check the integrity of a mesh.
struct MeshIntegrity
{
	/// @brief Status codes for mesh integrity check.
	enum struct Status : uint8_t
	{
		MeshOK = 0,
		VertexHasNullIncidentFace,
		VertexNotInFace,
		FaceHasNullVertex,
		FaceNeighborNotReciprocal,
	};

	/// @brief Check the integrity of the mesh.
	/// @param mesh The mesh to check.
	/// @return Status code indicating the result of the integrity check.
	static Status CheckIntegrity(const Data::Surface::Mesh& mesh);
};
} // namespace Utilitary::Mesh
