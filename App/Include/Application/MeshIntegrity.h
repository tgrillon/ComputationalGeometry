#pragma once

#include "Application/Mesh.h"

#include <cstdint>

namespace Utilitary::Surface
{
/// @brief Struct to check the integrity of a mesh.
struct MeshIntegrity
{
	/// @brief ExitCode codes for mesh integrity check.
	enum struct ExitCode : uint8_t
	{
		MeshOK = 0,
		VertexHasNullIncidentTriangle,
		VertexNotInTriangle,
		TriangleHasNullVertex,
		TriangleNeighborNotReciprocal,
		TriangleHasDuplicatedVertices,
		TriangleIsItsOwnNeighbor,
		InvalidVertexIndex,
		InvalidIncidentTriangleIndex,
		InvalidNeighborTriangleIndex,
	};

	/// @brief Check the integrity of the mesh.
	/// @param mesh The mesh to check.
	/// @return ExitCode indicating the result of the integrity check.
	static ExitCode CheckIntegrity(const Data::Surface::Mesh& mesh);
};
} // namespace Utilitary::Surface
