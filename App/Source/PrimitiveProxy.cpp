#include "Application/PrimitiveProxy.h"

using namespace BaseType;

namespace Data::Primitive
{
/*==================================FaceProxy Methods===================================*/
FaceProxy::FaceProxy(Data::Surface::Mesh& mesh, Data::Primitive::Face& face)
	: m_Mesh(&mesh)
	, m_Face(&face)
{}

bool FaceProxy::IsValid() const
{
	return m_Mesh && m_Face->Index < m_Mesh->GetFaceCount();
}

IndexType FaceProxy::GetIndex() const
{
	return m_Face->Index;
}

Vertex* FaceProxy::GetVertex(const uint8_t index) const
{
	assert(index < 3 && "[FaceProxy::GetVertex] Index out of bound");
	return m_Face->Vertices[index];
}

std::array<Vertex*, 3> FaceProxy::GetVertices() const
{
	return m_Face->Vertices;
}

Face* FaceProxy::GetNeighbor(const uint8_t index) const

{
	assert(index < 3 && "[FaceProxy::GetNeighbor] Index out of bound");
	return m_Face->Neighbors[index];
}

std::array<Face*, 3> FaceProxy::GetNeighbors() const
{
	return m_Face->Neighbors;
}

/*==================================VertexProxy Methods===================================*/
VertexProxy::VertexProxy(Data::Surface::Mesh& mesh, Data::Primitive::Vertex& vertex)
	: m_Mesh(&mesh)
	, m_Vertex(&vertex)
{}

bool VertexProxy::IsValid() const
{
	return m_Mesh && m_Vertex->Index < m_Mesh->GetVertexCount();
}

IndexType VertexProxy::GetIndex() const
{
	return m_Vertex->Index;
}

Vec3& VertexProxy::GetPosition()
{
	return m_Vertex->Position;
}

const Vec3& VertexProxy::GetPosition() const
{
	return m_Vertex->Position;
}

Face* VertexProxy::GetIncidentFace()
{
	return m_Vertex->IncidentFace;
}

const Face* VertexProxy::GetIncidentFace() const
{
	return m_Vertex->IncidentFace;
}

} // namespace Data::Primitive
