#pragma once

#include "Application/BaseType.h"
#include "Application/ExtraDataContainer.h"
#include "Application/Primitive.h"

#include <memory>
#include <vector>

/// Forward declaration
namespace Utilitary::Surface
{
class MeshExporter;
class MeshIntegrity;
class MeshLoader;
} // namespace Utilitary::Surface

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
	friend Utilitary::Surface::MeshExporter;
	friend Utilitary::Surface::MeshIntegrity;
	friend Utilitary::Surface::MeshLoader;
	friend Data::Primitive::FaceProxy;
	friend Data::Primitive::VertexProxy;

public:
	/// @brief Default ctor.
	Mesh() = default;
	/// @brief Copy ctor.
	Mesh(const Mesh& mesh);
	~Mesh() = default;

	/// @brief Deep clone of the mesh.
	std::unique_ptr<Mesh> Clone() const;

	/// @brief Get the number of vertices in the mesh.
	BaseType::IndexType GetVertexCount() const;

	/// @brief Get the number of faces in the mesh.
	BaseType::IndexType GetFaceCount() const;

	/// @brief Get a proxy to the vertex at the given index.
	Data::Primitive::VertexProxy GetVertex(const BaseType::IndexType index);
	/// @brief Get a proxy to the face at the given index.
	Data::Primitive::FaceProxy GetFace(const BaseType::IndexType index);

	/// @brief Get the vertex data at the given index.
	Data::Primitive::Vertex& GetVertexData(const BaseType::IndexType index);
	/// @brief Get the vertex data at the given index.
	const Data::Primitive::Vertex& GetVertexData(const BaseType::IndexType index) const;
	/// @brief Get the face data at the given index.
	Data::Primitive::Face& GetFaceData(const BaseType::IndexType index);
	/// @brief Get the face data at the given index.
	const Data::Primitive::Face& GetFaceData(const BaseType::IndexType index) const;

	/// @brief Add a vertex to the mesh and return its index.
	BaseType::IndexType AddVertex(const Data::Primitive::Vertex& vertex);
	/// @brief Add a face to the mesh and return its index.
	BaseType::IndexType AddFace(const Data::Primitive::Face& face);

	/// @brief Add extra data container for each vertex.
	void AddVerticesExtraDataContainer();
	/// @brief Add extra data container for each face.
	void AddFacesExtraDataContainer();

	/// @brief Get the vertices data.
	std::vector<Data::Primitive::Vertex>& GetVertices();
	/// @brief Get the vertices data.
	const std::vector<Data::Primitive::Vertex>& GetVertices() const;
	/// @brief Get the faces data.
	std::vector<Data::Primitive::Face>& GetFaces();
	/// @brief Get the faces data.
	const std::vector<Data::Primitive::Face>& GetFaces() const;

private:
	/// @brief List of vertices.
	std::vector<Data::Primitive::Vertex> m_Vertices{};
	/// @brief List of faces.
	std::vector<Data::Primitive::Face> m_Faces{};

	/// @brief Extra data containers for each vertex.
	std::vector<Data::Internal::ExtraDataContainer> m_VerticesExtraDataContainer{};
	/// @brief Extra data containers for each face.
	std::vector<Data::Internal::ExtraDataContainer> m_FacesExtraDataContainer{};
};
} // namespace Data::Surface
