#pragma once

#include "Application/BaseType.h"
#include "Application/ExtraDataContainer.h"
#include "Application/Primitive.h"

#include <memory>
#include <vector>

/// Forward declaration
namespace Utilitary
{
class MeshLoader;
}

namespace Data::Primitive
{
class FaceProxy;
class VertexProxy;
} // namespace Data::Primitive

namespace Data::Surface
{
/// @brief Class representing a 3D triangular mesh.
class Mesh
{
public:
	friend Utilitary::MeshLoader;
	friend Data::Primitive::FaceProxy;
	friend Data::Primitive::VertexProxy;

public:
	/// @brief Default ctor.
	Mesh() = default;
	/// @brief Copy ctor.
	Mesh(const Mesh& mesh);
	~Mesh() = default;

	/// @brief Deep clone of the mesh.
	std::unique_ptr<Mesh> Clone() const { return std::make_unique<Mesh>(*this); }

	/// @brief Get the number of vertices in the mesh.
	uint32_t GetVertexCount() const { return static_cast<uint32_t>(m_Vertices.size()); }

	/// @brief Get the number of faces in the mesh.
	uint32_t GetFaceCount() const { return static_cast<uint32_t>(m_Faces.size()); }

	/// @brief Get a proxy to the vertex at the given index.
	Data::Primitive::VertexProxy GetVertex(const BaseType::IndexType index);
	/// @brief Get a proxy to the face at the given index.
	Data::Primitive::FaceProxy GetFace(const BaseType::IndexType index);

	/// @brief Get the vertex data at the given index.
	Data::Primitive::Vertex& GetVertexData(const BaseType::IndexType index);
	/// @brief Get the face data at the given index.
	const Data::Primitive::Vertex& GetVertexData(const BaseType::IndexType index) const;
	/// @brief Get the face data at the given index.
	Data::Primitive::Face& GetFaceData(const BaseType::IndexType index);
	/// @brief Get the face data at the given index.
	const Data::Primitive::Face& GetFaceData(const BaseType::IndexType index) const;

private:
	/// @brief List of vertices.
	std::vector<Data::Primitive::Vertex> m_Vertices{};
	/// @brief List of faces.
	std::vector<Data::Primitive::Face> m_Faces{};

	/// @brief Extra data containers for each vertex.
	std::vector<Data::Internal::ExtraDataContainer> m_VertexExtraData{};
	/// @brief Extra data containers for each face.
	std::vector<Data::Internal::ExtraDataContainer> m_FaceExtraData{};
};
} // namespace Data::Surface
