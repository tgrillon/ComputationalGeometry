#include "Application/PrimitiveProxy.h"

#include <utility>

using namespace BaseType;

namespace Data::Primitive
{
/*==================================FaceProxy Methods===================================*/
FaceProxy::FaceProxy(Data::Surface::Mesh& mesh, const IndexType faceIdx)
	: m_Mesh(&mesh)
	, m_Index(faceIdx)
{}

bool FaceProxy::IsValid() const
{
	return m_Mesh && m_Index < m_Mesh->GetFaceCount();
}

IndexType FaceProxy::GetIndex() const
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

IndexType FaceProxy::GetVertex(const uint8_t index) const
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
VertexProxy::VertexProxy(Data::Surface::Mesh& mesh, const IndexType vertexIdx)
	: m_Mesh(&mesh)
	, m_Index(vertexIdx)
{}

// VertexProxy::VertexProxy(const VertexProxy& other)
// 	: m_Mesh(other.m_Mesh)
// 	, m_Index(other.m_Index)
// {}

// VertexProxy::VertexProxy(VertexProxy&& other)
// 	: m_Mesh(std::exchange(other.m_Mesh, nullptr))
// 	, m_Index(other.m_Index)
// {}

// VertexProxy& VertexProxy::operator=(const VertexProxy& other)
// {
// 	if(this != &other)
// 	{
// 		m_Mesh = other.m_Mesh;
// 		m_Index = other.m_Index;
// 	}

// 	return *this;
// }

// VertexProxy& VertexProxy::operator=(VertexProxy&& other)
// {
// 	if(this != &other)
// 	{
// 		m_Mesh = std::exchange(other.m_Mesh, nullptr);
// 		m_Index = other.m_Index;
// 	}

// 	return *this;
// }

bool VertexProxy::IsValid() const
{
	return m_Mesh && m_Index < m_Mesh->GetVertexCount();
}

IndexType VertexProxy::GetIndex() const
{
	return m_Mesh->m_Vertices[m_Index].Index;
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

int VertexProxy::GetIncidentFace()
{
	return GetVertex().IncidentFaceIdx;
}

const int VertexProxy::GetIncidentFace() const
{
	return GetVertex().IncidentFaceIdx;
}

} // namespace Data::Primitive
