#pragma once

#include "glm/glm.hpp"

#include <array>

namespace Data::Primitive
{
struct Triangle;

/// @brief Structure holding the informations relative to a vertex primitive.
/// @note IncidentFace is a pointer to one of the incident faces of the vertex.
struct Vertex
{
	/// @brief Vertex position in 3D.
	glm::vec3 Position;

	/// @brief Pointer to one of the incident faces for internal purposes.
	Triangle* IncidentFace;
};

/// @brief  Structure holding the informations relative to a triangular face primitive.
struct Triangle
{
	/// @brief Triangle vertices.
	std::array<Vertex*, 3> Vertices;

	/// @brief Triangle neighboring faces
	std::array<Triangle*, 3> Neighbors;
};
} // namespace Data::Primitive
