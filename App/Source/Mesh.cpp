#include "Application/Mesh.h"

#include "Application/PrimitiveProxy.h"

using namespace BaseType;
using namespace Data::Primitive;

namespace Data::Surface
{
Mesh::Mesh(const Mesh& other)
{
	m_Vertices.reserve(other.m_Vertices.size()); // Pre-allocate space
	m_Faces.reserve(other.m_Faces.size()); // Pre-allocate space

	for(const auto& vertex : other.m_Vertices)
		m_Vertices.emplace_back(vertex);
	for(const auto& face : other.m_Faces)
		m_Faces.emplace_back(face);
}

/// @brief Get the number of faces in the mesh.
std::unique_ptr<Mesh> Mesh::Clone() const
{
	return std::make_unique<Mesh>(*this);
}

uint32_t Mesh::GetVertexCount() const
{
	return static_cast<uint32_t>(m_Vertices.size());
}

uint32_t Mesh::GetFaceCount() const
{
	return static_cast<uint32_t>(m_Faces.size());
}

VertexProxy Mesh::GetVertex(const VertexIndex index)
{
	assert(index < GetVertexCount() && "Index out of bound");
	return VertexProxy(*this, index);
}

FaceProxy Mesh::GetFace(const FaceIndex index)
{
	assert(index < GetFaceCount() && "Index out of bound");
	return FaceProxy(*this, index);
}

const Vertex& Mesh::GetVertexData(const VertexIndex index) const
{
	assert(index < GetVertexCount() && "Index out of bound");
	return m_Vertices[index];
}

Vertex& Mesh::GetVertexData(const VertexIndex index)
{
	assert(index < GetVertexCount() && "Index out of bound");
	return m_Vertices[index];
}

const Face& Mesh::GetFaceData(const FaceIndex index) const
{
	assert(index < GetFaceCount() && "Index out of bound");
	return m_Faces[index];
}

Face& Mesh::GetFaceData(const FaceIndex index)
{
	assert(index < GetFaceCount() && "Index out of bound");
	return m_Faces[index];
}

VertexIndex Mesh::AddVertex(const Vertex& vertex)
{
	VertexIndex index = static_cast<VertexIndex>(m_Vertices.size());
	if(!m_Vertices.empty() && m_VerticesExtraDataContainer.size() == m_Vertices.size())
		m_VerticesExtraDataContainer.emplace_back();
	m_Vertices.emplace_back(vertex);
	return index;
}

FaceIndex Mesh::AddFace(const Face& face)
{
	FaceIndex index = static_cast<FaceIndex>(m_Faces.size());
	if(!m_Faces.empty() && m_FacesExtraDataContainer.size() == m_Faces.size())
		m_FacesExtraDataContainer.emplace_back();
	m_Faces.emplace_back(face);
	return index;
}

void Mesh::AddVerticesExtraDataContainer()
{
	m_VerticesExtraDataContainer.resize(GetVertexCount());
}

void Mesh::AddFacesExtraDataContainer()
{
	m_FacesExtraDataContainer.resize(GetFaceCount());
}

std::vector<Data::Primitive::Vertex>& Mesh::GetVertices()
{
	return m_Vertices;
}

const std::vector<Data::Primitive::Vertex>& Mesh::GetVertices() const
{
	return m_Vertices;
}

std::vector<Data::Primitive::Face>& Mesh::GetFaces()
{
	return m_Faces;
}

const std::vector<Data::Primitive::Face>& Mesh::GetFaces() const
{
	return m_Faces;
}
} // namespace Data::Surface
