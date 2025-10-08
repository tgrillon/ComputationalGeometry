#pragma once

#include "Application/MeshPrimitive.h"

#include <memory>
#include <vector>

/// Forward declaration
namespace Utilitary
{
class MeshLoader;
}

namespace Data::Surface
{

/// @brief Class representing a 3D triangular mesh.
class Mesh
{
public:
	friend Utilitary::MeshLoader;

public:
	/// @brief Default ctor.
	Mesh() = default;
	/// @brief Copy ctor.
	Mesh(const Mesh& mesh);
	~Mesh() = default;

	/// @brief Deep clone of the mesh.
	inline std::unique_ptr<Mesh> Clone() const { return std::make_unique<Mesh>(*this); }

	/// @brief Get the number of vertices in the mesh.
	inline uint32_t GetVertexCount() const { return static_cast<uint32_t>(m_Vertices.size()); }

	/// @brief Get the number of faces in the mesh.
	inline uint32_t GetFaceCount() const { return static_cast<uint32_t>(m_Faces.size()); }

private:
	/// @brief List of vertices.
	std::vector<std::unique_ptr<Primitive::Vertex>> m_Vertices{};
	/// @brief List of faces.
	std::vector<std::unique_ptr<Primitive::Triangle>> m_Faces{};
};
} // namespace Data::Surface
