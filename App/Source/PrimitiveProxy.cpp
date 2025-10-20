#include "Application/PrimitiveProxy.h"

#include <utility>

using namespace BaseType;

namespace Data::Primitive
{
/*==================================TriangleProxy Methods===================================*/
TriangleProxy::TriangleProxy(Data::Surface::Mesh& mesh, const TriangleIndex index)
	: m_Mesh(&mesh)
	, m_Index(index)
{}

TriangleIndex TriangleProxy::GetIndex() const
{
	return m_Index;
}

Triangle& TriangleProxy::GetTriangle()
{
	return m_Mesh->m_Triangles[m_Index];
}

const Triangle& TriangleProxy::GetTriangle() const
{
	return m_Mesh->m_Triangles[m_Index];
}

VertexIndex TriangleProxy::GetVertex(const EdgeIndex index) const
{
	assert(index < 3 && "[TriangleProxy::GetVertex] Index out of bound");
	return GetTriangle().Vertices[index];
}

std::array<int, 3> TriangleProxy::GetVertices() const
{
	return GetTriangle().Vertices;
}

int TriangleProxy::GetNeighbor(const EdgeIndex index) const
{
	assert(index < 3 && "[TriangleProxy::GetNeighbor] Index out of bound");
	return GetTriangle().Neighbors[index];
}

std::array<int, 3> TriangleProxy::GetNeighbors() const
{
	return GetTriangle().Neighbors;
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

int VertexProxy::GetIncidentTriangle() const
{
	return GetVertex().IncidentTriangleIdx;
}
} // namespace Data::Primitive
