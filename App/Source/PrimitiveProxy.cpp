#include "Application/PrimitiveProxy.h"

#include <utility>

using namespace BaseType;

namespace Data::Primitive
{
/*==================================FaceProxy Methods===================================*/
FaceProxy::FaceProxy(Data::Surface::Mesh& mesh, const FaceIndex index)
	: m_Mesh(&mesh)
	, m_Index(index)
{}

FaceIndex FaceProxy::GetIndex() const
{
	return m_Index;
}

Face& FaceProxy::GetFace()
{
	return m_Mesh->m_Faces[m_Index];
}

const Face& FaceProxy::GetFace() const
{
	return m_Mesh->m_Faces[m_Index];
}

VertexIndex FaceProxy::GetVertex(const EdgeIndex index) const
{
	assert(index < 3 && "[FaceProxy::GetVertex] Index out of bound");
	return GetFace().Vertices[index];
}

std::array<int, 3> FaceProxy::GetVertices() const
{
	return GetFace().Vertices;
}

int FaceProxy::GetNeighbor(const uint8_t index) const
{
	assert(index < 3 && "[FaceProxy::GetNeighbor] Index out of bound");
	return GetFace().Neighbors[index];
}

std::array<int, 3> FaceProxy::GetNeighbors() const
{
	return GetFace().Neighbors;
}

/*==================================VertexProxy Methods===================================*/

VertexProxy::VertexProxy(Data::Surface::Mesh& mesh, const VertexIndex index)
	: m_Mesh(&mesh)
	, m_Index(index)
{}

VertexIndex VertexProxy::GetIndex() const
{
	return m_Index;
}

Vertex& VertexProxy::GetVertex()
{
	return m_Mesh->m_Vertices[m_Index];
}

const Vertex& VertexProxy::GetVertex() const
{
	return m_Mesh->m_Vertices[m_Index];
}

Vec3& VertexProxy::GetPosition()
{
	return GetVertex().Position;
}

const Vec3& VertexProxy::GetPosition() const
{
	return GetVertex().Position;
}

int VertexProxy::GetIncidentFace() const
{
	return GetVertex().IncidentFaceIdx;
}
} // namespace Data::Primitive
