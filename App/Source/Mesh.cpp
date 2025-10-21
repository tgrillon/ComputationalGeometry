#include "Application/Mesh.h"

#include "Application/ExtraDataType.h"
#include "Application/MathHelpers.h"
#include "Application/PrimitiveProxy.h"
#include "Application/VertexPair.h"

using namespace BaseType;
using namespace Math::Geometry;
using namespace Data::Primitive;
using namespace Data::ExtraData;
using namespace Utilitary::Primitive;

#include <numeric>

namespace Data::Surface
{
Mesh::Mesh(const Mesh& other)
	: m_Vertices(other.m_Vertices)
	, m_Triangles(other.m_Triangles)
	, m_VerticesExtraDataContainer(other.m_VerticesExtraDataContainer)
	, m_TrianglesExtraDataContainer(other.m_TrianglesExtraDataContainer)
{}

/// @brief Get the number of faces in the mesh.
std::unique_ptr<Mesh> Mesh::Clone() const
{
	return std::make_unique<Mesh>(*this);
}

uint32_t Mesh::GetVertexCount() const
{
	return static_cast<uint32_t>(m_Vertices.size());
}

uint32_t Mesh::GetTriangleCount() const
{
	return static_cast<uint32_t>(m_Triangles.size());
}

VertexProxy Mesh::GetVertex(const VertexIndex index)
{
	assert(index < GetVertexCount() && "Index out of bound");
	return VertexProxy(*this, index);
}

TriangleProxy Mesh::GetTriangle(const TriangleIndex index)
{
	assert(index < GetTriangleCount() && "Index out of bound");
	return TriangleProxy(*this, index);
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

const Triangle& Mesh::GetTriangleData(const TriangleIndex index) const
{
	assert(index < GetTriangleCount() && "Index out of bound");
	return m_Triangles[index];
}

Triangle& Mesh::GetTriangleData(const TriangleIndex index)
{
	assert(index < GetTriangleCount() && "Index out of bound");
	return m_Triangles[index];
}

VertexIndex Mesh::AddVertex(const Vertex& vertex)
{
	VertexIndex index = static_cast<VertexIndex>(m_Vertices.size());
	if(!m_Vertices.empty() && m_VerticesExtraDataContainer.size() == m_Vertices.size())
		m_VerticesExtraDataContainer.emplace_back();
	m_Vertices.emplace_back(vertex);
	return index;
}

TriangleIndex Mesh::AddTriangle(const Triangle& triangle)
{
	TriangleIndex index = static_cast<TriangleIndex>(m_Triangles.size());
	if(!m_Triangles.empty() && m_TrianglesExtraDataContainer.size() == m_Triangles.size())
		m_TrianglesExtraDataContainer.emplace_back();
	m_Triangles.emplace_back(triangle);
	return index;
}

void Mesh::AddVerticesExtraDataContainer()
{
	if(!m_VerticesExtraDataContainer.empty())
		m_VerticesExtraDataContainer.clear();
	m_VerticesExtraDataContainer.resize(GetVertexCount());
}

void Mesh::AddTrianglesExtraDataContainer()
{
	if(!m_TrianglesExtraDataContainer.empty())
		m_TrianglesExtraDataContainer.clear();
	m_TrianglesExtraDataContainer.resize(GetTriangleCount());
}

void Mesh::UpdateMeshConnectivity()
{
	std::unordered_map<VertexPair, std::pair<TriangleIndex, EdgeIndex>> neighborMap;

	for(TriangleIndex iTriangle = 0; iTriangle < GetTriangleCount(); ++iTriangle)
	{
		Triangle& curTriangle = m_Triangles[iTriangle];

		// Set the incident triangle index for each vertex if it's not already the case.
		for(EdgeIndex iEdge = 0; iEdge < 3; ++iEdge)
		{
			int curVertexIdx = curTriangle.Vertices[iEdge];
			assert(curVertexIdx != -1);

			Vertex& curVertex = m_Vertices[curVertexIdx];

			if(curVertex.IncidentTriangleIdx == -1)
				curVertex.IncidentTriangleIdx = iTriangle;
		}

		// Set neighboring faces using the edges
		auto SetFacesNeigbhor = [&](VertexIndex firstVertexIdx, VertexIndex secondVertexIdx, uint8_t edgeIdx)
		{
			if(neighborMap.find({ firstVertexIdx, secondVertexIdx }) == neighborMap.end())
			{
				neighborMap[{ firstVertexIdx, secondVertexIdx }] = { iTriangle, edgeIdx };
			}
			else // The edge with firstVertex and secondVertex is already registered in map
			{
				auto [faceNeighborIdx, neighborEdgeIdx] = neighborMap[{ firstVertexIdx, secondVertexIdx }];
				m_Triangles[faceNeighborIdx].Neighbors[neighborEdgeIdx] = iTriangle;
				curTriangle.Neighbors[edgeIdx] = faceNeighborIdx;
			}
		};

		const VertexIndex v0Idx = static_cast<VertexIndex>(curTriangle.Vertices[0]);
		const VertexIndex v1Idx = static_cast<VertexIndex>(curTriangle.Vertices[1]);
		const VertexIndex v2Idx = static_cast<VertexIndex>(curTriangle.Vertices[2]);

		// Edge v0-v1
		SetFacesNeigbhor(v0Idx, v1Idx, 2);
		// Edge v1-v2
		SetFacesNeigbhor(v1Idx, v2Idx, 0);
		// Edge v2-v0
		SetFacesNeigbhor(v2Idx, v0Idx, 1);
	}
}

std::vector<Data::Primitive::Vertex>& Mesh::GetVertices()
{
	return m_Vertices;
}

const std::vector<Data::Primitive::Vertex>& Mesh::GetVertices() const
{
	return m_Vertices;
}

std::vector<Data::Primitive::Triangle>& Mesh::GetTriangles()
{
	return m_Triangles;
}

const std::vector<Data::Primitive::Triangle>& Mesh::GetTriangles() const
{
	return m_Triangles;
}

bool Mesh::HasVerticesExtraDataContainer() const
{
	return m_VerticesExtraDataContainer.size() > 0;
}

bool Mesh::HasTrianglesExtraDataContainer() const
{
	return m_TrianglesExtraDataContainer.size() > 0;
}

void Mesh::ComputeTriangleNormals(bool normalize)
{
	if(!HasTrianglesExtraDataContainer())
		AddTrianglesExtraDataContainer();

	// if(computeSmoothVertexNormals && m_VerticesExtraDataContainer.empty())
	// 	AddVerticesExtraDataContainer();

	for(TriangleIndex iTriangle = 0; iTriangle < GetTriangleCount(); ++iTriangle)
	{
		const TriangleProxy& curTriangle = GetTriangle(iTriangle);

		const Vec3& posA = m_Vertices[curTriangle.GetVertex(0)].Position;
		const Vec3& posB = m_Vertices[curTriangle.GetVertex(1)].Position;
		const Vec3& posC = m_Vertices[curTriangle.GetVertex(2)].Position;

		const Vec3 AB = Normalize(posB - posA);
		const Vec3 AC = Normalize(posC - posA);

		TriangleNormalExtraData& curTriangleNormal = curTriangle.GetOrCreateExtraData<TriangleNormalExtraData>();
		Vec3 computedNormal = Cross(AB, AC);
		if(normalize)
			computedNormal = Normalize(computedNormal);
		curTriangleNormal.SetData(computedNormal);

		// if(computeSmoothVertexNormals)
		// {
		// 	// Compute the remaining vectors of the triangle.
		// 	const Vec3 BC = glm::normalize(posC - posB);
		// 	const Vec3 BA = glm::normalize(posA - posB);

		// 	const Vec3 CA = glm::normalize(posA - posC);
		// 	const Vec3 CB = glm::normalize(posB - posC);

		// 	// Compute each angle (in randian).
		// 	const float angleA = glm::angle(AB, AC);
		// 	const float angleB = glm::angle(BC, BA);
		// 	const float angleC = glm::angle(CA, CB);

		// 	// Get a proxy on each vertex of the triangle.
		// 	const VertexProxy& vertexA = GetVertex(curTriangle.GetVertex(0));
		// 	const VertexProxy& vertexB = GetVertex(curTriangle.GetVertex(1));
		// 	const VertexProxy& vertexC = GetVertex(curTriangle.GetVertex(2));

		// 	// Add the normal weighted by the related angle as an extra data to each vertex.
		// 	FlatVertexNormalsExtraData& vertexAExtraData = vertexA.GetOrCreateExtraData<FlatVertexNormalsExtraData>();
		// 	vertexAExtraData.GetData().emplace_back(computedNormal * angleA);
		// 	FlatVertexNormalsExtraData& vertexBExtraData = vertexB.GetOrCreateExtraData<FlatVertexNormalsExtraData>();
		// 	vertexBExtraData.GetData().emplace_back(computedNormal * angleB);
		// 	FlatVertexNormalsExtraData& vertexCExtraData = vertexC.GetOrCreateExtraData<FlatVertexNormalsExtraData>();
		// 	vertexCExtraData.GetData().emplace_back(computedNormal * angleC);
		// }
	}

	// if(computeSmoothVertexNormals)
	// {
	// 	// Compute the smooth normal for each vertex of the mesh.
	// 	for(VertexIndex iVertex = 0; iVertex < GetVertexCount(); ++iVertex)
	// 	{
	// 		// Get the current vertex and create the extra data that will handle the smooth vertex normal.
	// 		const VertexProxy& curVertex = GetVertex(iVertex);
	// 		SmoothVertexNormalExtraData& curVertexNormal = curVertex.GetOrCreateExtraData<SmoothVertexNormalExtraData>();

	// 		// Get the precomputed flat vertex normals.
	// 		auto vertexFlatNormals = curVertex.GetExtraData<FlatVertexNormalsExtraData>();
	// 		assert(vertexFlatNormals != nullptr);

	// 		// Accumulate the flat vertex normals.
	// 		Vec3 computedNormal = std::accumulate(
	// 			vertexFlatNormals->GetData().begin(),
	// 			vertexFlatNormals->GetData().end(),
	// 			Vec3{ 0., 0., 0. },
	// 			std::plus<Vec3>());

	// 		curVertexNormal.SetData(glm::normalize(computedNormal));
	// 		curVertex.EraseExtraData<FlatVertexNormalsExtraData>();
	// 	}
	// }
}

void Mesh::ComputeSmoothVertexNormals(bool normalize)
{
	if(!HasTrianglesExtraDataContainer())
		AddTrianglesExtraDataContainer();

	if(!HasVerticesExtraDataContainer())
		AddVerticesExtraDataContainer();

	for(TriangleIndex iTriangle = 0; iTriangle < GetTriangleCount(); ++iTriangle)
	{
		const TriangleProxy& curTriangle = GetTriangle(iTriangle);

		const Vec3& posA = m_Vertices[curTriangle.GetVertex(0)].Position;
		const Vec3& posB = m_Vertices[curTriangle.GetVertex(1)].Position;
		const Vec3& posC = m_Vertices[curTriangle.GetVertex(2)].Position;

		const Vec3 AB = Normalize(posB - posA);
		const Vec3 AC = Normalize(posC - posA);

		const Vec3 BC = glm::normalize(posC - posB);
		const Vec3 BA = glm::normalize(posA - posB);

		const Vec3 CA = glm::normalize(posA - posC);
		const Vec3 CB = glm::normalize(posB - posC);

		Vec3 curTriangleNormal;
		if(curTriangle.HasExtraData<TriangleNormalExtraData>())
		{
			curTriangleNormal = curTriangle.GetExtraData<TriangleNormalExtraData>()->GetData();
		}
		else
		{
			curTriangleNormal = Cross(AB, AC);
		}

		curTriangleNormal = Normalize(curTriangleNormal);

		// Compute each angle (in randian).
		const float angleA = Angle(AB, AC);
		const float angleB = Angle(BC, BA);
		const float angleC = Angle(CA, CB);

		// Get a proxy on each vertex of the triangle.
		const VertexProxy& vertexA = GetVertex(curTriangle.GetVertex(0));
		const VertexProxy& vertexB = GetVertex(curTriangle.GetVertex(1));
		const VertexProxy& vertexC = GetVertex(curTriangle.GetVertex(2));

		// Add the normal weighted by the related angle as an extra data to each vertex.
		FlatVertexNormalsExtraData& vertexAExtraData = vertexA.GetOrCreateExtraData<FlatVertexNormalsExtraData>();
		vertexAExtraData.GetData().emplace_back(curTriangleNormal * angleA);
		FlatVertexNormalsExtraData& vertexBExtraData = vertexB.GetOrCreateExtraData<FlatVertexNormalsExtraData>();
		vertexBExtraData.GetData().emplace_back(curTriangleNormal * angleB);
		FlatVertexNormalsExtraData& vertexCExtraData = vertexC.GetOrCreateExtraData<FlatVertexNormalsExtraData>();
		vertexCExtraData.GetData().emplace_back(curTriangleNormal * angleC);
	}

	// Compute the smooth normal for each vertex of the mesh.
	for(VertexIndex iVertex = 0; iVertex < GetVertexCount(); ++iVertex)
	{
		// Get the current vertex and create the extra data that will handle the smooth vertex normal.
		const VertexProxy& curVertex = GetVertex(iVertex);
		SmoothVertexNormalExtraData& curVertexNormal = curVertex.GetOrCreateExtraData<SmoothVertexNormalExtraData>();

		// Get the precomputed flat vertex normals.
		auto vertexFlatNormals = curVertex.GetExtraData<FlatVertexNormalsExtraData>();
		assert(vertexFlatNormals != nullptr);

		// Accumulate the flat vertex normals.
		Vec3 computedNormal = std::accumulate(
			vertexFlatNormals->GetData().begin(),
			vertexFlatNormals->GetData().end(),
			Vec3{ 0., 0., 0. },
			std::plus<Vec3>());

		if(normalize)
			computedNormal = Normalize(computedNormal);
		curVertexNormal.SetData(computedNormal);
		curVertex.EraseExtraData<FlatVertexNormalsExtraData>();
	}
}
} // namespace Data::Surface
