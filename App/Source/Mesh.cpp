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

VertexProxy Mesh::GetVertex(const IndexType index)
{
	assert(index < GetVertexCount() && "[Mesh::GetVertex] Index out of bound");
	return VertexProxy(*this, m_Vertices[index]);
}

FaceProxy Mesh::GetFace(const IndexType index)
{
	assert(index < GetFaceCount() && "[Mesh::GetFace] Index out of bound");
	return FaceProxy(*this, m_Faces[index]);
}

const Vertex& Mesh::GetVertexData(const IndexType index) const
{
	assert(index < GetVertexCount() && "[Mesh::GetVertexData] Index out of bound");
	return m_Vertices[index];
}

const Face& Mesh::GetFaceData(const IndexType index) const
{
	assert(index < GetFaceCount() && "[Mesh::GetFaceData] Index out of bound");
	return m_Faces[index];
}

IndexType Mesh::AddVertex(const Vertex& vertex)
{
	IndexType index = static_cast<IndexType>(m_Vertices.size());
	m_Vertices.emplace_back(vertex);
	m_VertexExtraData.emplace_back();
	return index;
}

BaseType::IndexType Mesh::AddFace(const Face& face)
{
	IndexType index = static_cast<IndexType>(m_Faces.size());
	m_Faces.emplace_back(face);
	m_FaceExtraData.emplace_back();
	return index;
}
} // namespace Data::Surface
