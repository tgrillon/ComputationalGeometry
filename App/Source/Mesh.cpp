#include "Application/Mesh.h"

#include "Application/ExtraDataType.h"
#include "Application/PrimitiveProxy.h"
#include "Application/VertexPair.h"

using namespace BaseType;
using namespace Data::Primitive;
using namespace Data::ExtraData;
using namespace Utilitary::Primitive;

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

#include <numeric>

namespace Data::Surface
{
Mesh::Mesh(const Mesh& other)
	: m_Vertices(other.m_Vertices)
	, m_Faces(other.m_Faces)
	, m_VerticesExtraDataContainer(other.m_VerticesExtraDataContainer)
	, m_FacesExtraDataContainer(other.m_FacesExtraDataContainer)
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

void Mesh::UpdateMeshConnectivity()
{
	std::unordered_map<VertexPair, std::pair<FaceIndex, EdgeIndex>> neighborMap;

	for(FaceIndex iFace = 0; iFace < GetFaceCount(); ++iFace)
	{
		Face& curFace = m_Faces[iFace];

		// Set the incident face index for each vertex if it's not already the case.
		for(EdgeIndex iEdge = 0; iEdge < 3; ++iEdge)
		{
			int curVertexIdx = curFace.Vertices[iEdge];
			assert(curVertexIdx != -1);

			Vertex& curVertex = m_Vertices[curVertexIdx];

			if(curVertex.IncidentFaceIdx == -1)
				curVertex.IncidentFaceIdx = iFace;
		}

		// Set neighboring faces using the edges
		auto SetFacesNeigbhor = [&](VertexIndex firstVertexIdx, VertexIndex secondVertexIdx, uint8_t edgeIdx)
		{
			if(neighborMap.find({ firstVertexIdx, secondVertexIdx }) == neighborMap.end())
			{
				neighborMap[{ firstVertexIdx, secondVertexIdx }] = { iFace, edgeIdx };
			}
			else // The edge with firstVertex and secondVertex is already registered in map
			{
				auto [faceNeighborIdx, neighborEdgeIdx] = neighborMap[{ firstVertexIdx, secondVertexIdx }];
				m_Faces[faceNeighborIdx].Neighbors[neighborEdgeIdx] = iFace;
				curFace.Neighbors[edgeIdx] = faceNeighborIdx;
			}
		};

		const VertexIndex v0Idx = static_cast<VertexIndex>(curFace.Vertices[0]);
		const VertexIndex v1Idx = static_cast<VertexIndex>(curFace.Vertices[1]);
		const VertexIndex v2Idx = static_cast<VertexIndex>(curFace.Vertices[2]);

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

std::vector<Data::Primitive::Face>& Mesh::GetFaces()
{
	return m_Faces;
}

const std::vector<Data::Primitive::Face>& Mesh::GetFaces() const
{
	return m_Faces;
}

bool Mesh::HasVerticesExtraDataContainer() const
{
	return m_VerticesExtraDataContainer.size() > 0;
}

bool Mesh::HasFacesExtraDataContainer() const
{
	return m_FacesExtraDataContainer.size() > 0;
}

void Mesh::ComputeFaceNormals(bool computeSmoothVertexNormals)
{
	if(m_FacesExtraDataContainer.empty())
		AddFacesExtraDataContainer();

	if(computeSmoothVertexNormals && m_VerticesExtraDataContainer.empty())
		AddVerticesExtraDataContainer();

	for(FaceIndex iFace = 0; iFace < GetFaceCount(); ++iFace)
	{
		const FaceProxy& curFace = GetFace(iFace);

		const Vec3& posA = m_Vertices[curFace.GetVertex(0)].Position;
		const Vec3& posB = m_Vertices[curFace.GetVertex(1)].Position;
		const Vec3& posC = m_Vertices[curFace.GetVertex(2)].Position;

		const Vec3 AB = glm::normalize(posB - posA);
		const Vec3 AC = glm::normalize(posC - posA);

		FaceNormalExtraData& curFaceNormal = curFace.GetOrCreateExtraData<FaceNormalExtraData>();
		const Vec3& computedNormal = glm::normalize(glm::cross(AB, AC));
		curFaceNormal.SetData(computedNormal);

		if(computeSmoothVertexNormals)
		{
			// Compute the remaining vectors of the triangle.
			const Vec3 BC = glm::normalize(posC - posB);
			const Vec3 BA = glm::normalize(posA - posB);

			const Vec3 CA = glm::normalize(posA - posC);
			const Vec3 CB = glm::normalize(posB - posC);

			// Compute each angle (in randian).
			const float angleA = glm::angle(AB, AC);
			const float angleB = glm::angle(BC, BA);
			const float angleC = glm::angle(CA, CB);

			// Get a proxy on each vertex of the triangle.
			const VertexProxy& vertexA = GetVertex(curFace.GetVertex(0));
			const VertexProxy& vertexB = GetVertex(curFace.GetVertex(1));
			const VertexProxy& vertexC = GetVertex(curFace.GetVertex(2));

			// Add the normal weighted by the related angle as an extra data to each vertex.
			FlatVertexNormalsExtraData& vertexAExtraData = vertexA.GetOrCreateExtraData<FlatVertexNormalsExtraData>();
			vertexAExtraData.GetData().emplace_back(computedNormal * angleA);
			FlatVertexNormalsExtraData& vertexBExtraData = vertexB.GetOrCreateExtraData<FlatVertexNormalsExtraData>();
			vertexBExtraData.GetData().emplace_back(computedNormal * angleB);
			FlatVertexNormalsExtraData& vertexCExtraData = vertexC.GetOrCreateExtraData<FlatVertexNormalsExtraData>();
			vertexCExtraData.GetData().emplace_back(computedNormal * angleC);
		}
	}

	if(computeSmoothVertexNormals)
	{
		// Compute the smooth normal for each vertex of the mesh.
		for(VertexIndex iVertex = 0; iVertex < GetVertexCount(); ++iVertex)
		{
			// Get the current vertex and create the extra data that will handle the smooth vertex normal.
			const VertexProxy& curVertex = GetVertex(iVertex);
			VertexNormalExtraData& curVertexNormal = curVertex.GetOrCreateExtraData<VertexNormalExtraData>();

			// Get the precomputed flat vertex normals.
			auto vertexFlatNormals = curVertex.GetExtraData<FlatVertexNormalsExtraData>();
			assert(vertexFlatNormals != nullptr);

			// Accumulate the flat vertex normals.
			Vec3 computedNormal = std::accumulate(
				vertexFlatNormals->GetData().begin(),
				vertexFlatNormals->GetData().end(),
				Vec3{ 0., 0., 0. },
				std::plus<Vec3>());

			curVertexNormal.SetData(glm::normalize(computedNormal));
			curVertex.EraseExtraData<FlatVertexNormalsExtraData>();
		}
	}
}
} // namespace Data::Surface
