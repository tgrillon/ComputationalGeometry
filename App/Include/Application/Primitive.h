#pragma once

#include "Application/BaseType.h"

#include <array>

/// @brief Helper namespace for index manipulations in triangular faces.
namespace IndexHelpers
{
constexpr std::array<uint8_t, 3> Previous{ 2, 0, 1 };
constexpr std::array<uint8_t, 3> Current{ 0, 1, 2 };
constexpr std::array<uint8_t, 3> Next{ 1, 2, 0 };
} // namespace IndexHelpers

namespace Data::Primitive
{
struct Face;

/// @brief Structure holding the informations relative to a vertex primitive.
/// @note IncidentFace is a pointer to one of the incident faces of the vertex.
struct Vertex
{
	/// @brief Unique index of the vertex in the mesh.
	BaseType::IndexType Index{};

	/// @brief Vertex position in 3D.
	BaseType::Vec3 Position{};

	/// @brief Index to one of the incident faces for internal purposes.
	/// @note If the vertex has no incident face, this index is set to -1.
	int IncidentFaceIdx{ -1 };
};

/// @brief  Structure holding the informations relative to a triangular face primitive.
struct Face
{
	/// @brief Unique index of the triangle in the mesh.
	BaseType::IndexType Index{};

	/// @brief Face vertex indices.
	/// @note Vertices must be stored in counter-clockwise order.
	/// @note The face wont be valid if one of the indices is -1.
	std::array<int, 3> Vertices{ -1, -1, -1 };

	/// @brief Face neighboring face indices.
	/// @note Neighbors[i] is the neighbor opposite to Vertices[i].
	/// @note If the face has no neighbor on the edge, the index is set to -1.
	std::array<int, 3> Neighbors{ -1, -1, -1 };
};
} // namespace Data::Primitive

namespace Utilitary::Primitive
{
/// @brief Get the index of the edge defined by two vertices in a face.
/// @param face The face containing the edge.
/// @param v0 The first vertex of the edge.
/// @param v1 The second vertex of the edge.
/// @return The index of the edge (0, 1, or 2), or -1 if the edge is not found.
int8_t GetEdgeIndex(const Data::Primitive::Face& face, const Data::Primitive::Vertex& v0, const Data::Primitive::Vertex& v1);
} // namespace Utilitary::Primitive
