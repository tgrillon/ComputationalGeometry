#include "Application/Mesh.h"

#include "Application/PrimitiveProxy.h"
#include "Application/VertexPair.h"

using namespace BaseType;
using namespace Data::Primitive;
using namespace Utilitary::Primitive;

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

bool Mesh::HasVertexExtraData() const
{
	return m_VerticesExtraDataContainer.size() > 0;
}

bool Mesh::HasFaceExtraData() const
{
	return m_FacesExtraDataContainer.size() > 0;
}

//==========================VerticesAroundVertexCirculator==========================//
Mesh::VerticesAroundVertexCirculator::VerticesAroundVertexCirculator(const Mesh& mesh, const VertexIndex index)
	: m_Mesh(mesh)
	, m_CentralVertexIdx(index)
{
	// Get the vertex data of the central vertex.
	const Vertex& curVertex = m_Mesh.GetVertexData(m_CentralVertexIdx);
	assert(curVertex.IncidentFaceIdx != -1); // Its incident face index must be set.

	// Set the current face index.
	m_CurFaceIdx = curVertex.IncidentFaceIdx;

	// Get the current face data.
	const Face& curFace = m_Mesh.GetFaceData(m_CurFaceIdx);
	// Get the local index of the central mesh in the current face.
	int localIdx = GetVertexLocalIndex(curFace, m_CentralVertexIdx);
	assert(localIdx != -1); // The current face must contains the central vertex.

	//  Set the current vertex index.
	m_CurVertexLocalIdx = IndexHelpers::Previous[localIdx];
	m_CurVertexIdx = curFace.Vertices[m_CurVertexLocalIdx];
}

bool Mesh::VerticesAroundVertexCirculator::operator==(const Mesh::VerticesAroundVertexCirculator& rhs) const
{
	// Exit for counter clock-wise travel (case where the vertex neighborhood form a ring)
	bool hasCompletedFullCirculation = m_IsInCCWOrder && m_IsActive && m_CurVertexIdx == rhs.m_CurVertexIdx;
	// Exit for clock-wise travel (case where the vertex neighborhood is open and has a boundary)
	bool hasReachedBoundaryEnd = !m_IsInCCWOrder && m_CurFaceIdx == -1;
	return hasCompletedFullCirculation || hasReachedBoundaryEnd;
}

bool Mesh::VerticesAroundVertexCirculator::operator!=(const Mesh::VerticesAroundVertexCirculator& rhs) const
{
	return !operator==(rhs);
}

Mesh::VerticesAroundVertexCirculator& Mesh::VerticesAroundVertexCirculator::operator++()
{
	m_IsActive = true;

	int neighborFaceIdx;
	{
		const Face& curFace = m_Mesh.GetFaceData(m_CurFaceIdx);
		neighborFaceIdx = curFace.Neighbors[IndexHelpers::Previous[m_CurVertexLocalIdx]];
	}

	if(m_IsInCCWOrder)
	{
		if(neighborFaceIdx == -1)
		{ // If there is no next face, we reached a boundary.
			while(m_JumpCount > 0)
			{ // We need to go back to the previous face and start going in the opposite direction.
				const Face& curFace = m_Mesh.GetFaceData(m_CurFaceIdx);
				int localIdx = GetVertexLocalIndex(curFace, m_CentralVertexIdx);
				assert(localIdx != -1);
				m_PrevFaceIdx = m_CurFaceIdx;
				m_CurFaceIdx = curFace.Neighbors[IndexHelpers::Previous[localIdx]];
				m_JumpCount--;
			}

			// We are now going in the clock-wise direction.
			m_IsInCCWOrder = false;
			UpdateCurVertexIndexInCWOrder();
		}
		else // We are still going in the counter clock-wise direction.
		{
			// Update the jump count.
			m_JumpCount++;
			m_PrevFaceIdx = m_CurFaceIdx;
			m_CurFaceIdx = neighborFaceIdx;

			UpdateCurVertexIndexInCCWOrder();
		}
	}
	else // We are going in the clock-wise direction.
	{
		m_PrevFaceIdx = m_CurFaceIdx;
		const Face& prevFace = m_Mesh.GetFaceData(m_CurFaceIdx);
		m_CurFaceIdx = prevFace.Neighbors[IndexHelpers::Next[m_CurVertexLocalIdx]];
		if(m_CurFaceIdx == -1)
			return *this; // We reached a boundary, we stop here.

		UpdateCurVertexIndexInCWOrder();
	}

	return *this;
}

Mesh::VerticesAroundVertexCirculator Mesh::VerticesAroundVertexCirculator::operator++(int)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

VertexIndex Mesh::VerticesAroundVertexCirculator::operator*() const
{
	return m_CurVertexIdx;
}

void Mesh::VerticesAroundVertexCirculator::SetIsActive(bool value)
{
	m_IsActive = value;
}

void Mesh::VerticesAroundVertexCirculator::UpdateCurVertexIndexInCWOrder()
{
	const Face& curFace = m_Mesh.GetFaceData(m_CurFaceIdx);
	int localIdx = GetVertexLocalIndex(curFace, m_CentralVertexIdx);
	assert(localIdx != -1);

	// Update the vertex and face informations.
	m_CurVertexLocalIdx = IndexHelpers::Next[localIdx];
	m_CurVertexIdx = curFace.Vertices[m_CurVertexLocalIdx];
}

void Mesh::VerticesAroundVertexCirculator::UpdateCurVertexIndexInCCWOrder()
{
	const Face& curFace = m_Mesh.GetFaceData(m_CurFaceIdx);
	int localIdx = GetVertexLocalIndex(curFace, m_CentralVertexIdx);
	assert(localIdx != -1);

	// Update the vertex and face informations.
	m_CurVertexLocalIdx = IndexHelpers::Previous[localIdx];
	m_CurVertexIdx = curFace.Vertices[m_CurVertexLocalIdx];
}

//==========================VerticesAroundVertexRange==========================//
Mesh::VerticesAroundVertexRange::VerticesAroundVertexRange(const Mesh& mesh, const VertexIndex index)
	: m_Mesh(mesh)
	, m_VertexIdx(index)
{}

Mesh::VerticesAroundVertexCirculator Mesh::VerticesAroundVertexRange::begin() const
{
	Mesh::VerticesAroundVertexCirculator circ(m_Mesh, m_VertexIdx);
	circ.SetIsActive(false);
	return circ;
}

Mesh::VerticesAroundVertexCirculator Mesh::VerticesAroundVertexRange::end() const
{
	Mesh::VerticesAroundVertexCirculator circ(m_Mesh, m_VertexIdx);
	circ.SetIsActive(true);
	return circ;
}

Mesh::VerticesAroundVertexRange Mesh::GetVerticesAroundVertex(const VertexIndex index) const
{
	return VerticesAroundVertexRange(*this, index);
}

//==========================FacesAroundVertexCirculator==========================//
Mesh::FacesAroundVertexCirculator::FacesAroundVertexCirculator(const Mesh& mesh, const VertexIndex index)
	: m_Mesh(mesh)
	, m_CentralVertexIdx(index)
{
	// Get the vertex data of the central vertex.
	const Vertex& curVertex = m_Mesh.GetVertexData(m_CentralVertexIdx);
	assert(curVertex.IncidentFaceIdx != -1); // Its incident face index must be set.

	// Set the current face index.
	m_CurFaceIdx = curVertex.IncidentFaceIdx;
	m_PrevFaceIdx = -1;
	m_JumpCount++;
}

bool Mesh::FacesAroundVertexCirculator::operator==(const Mesh::FacesAroundVertexCirculator& rhs) const
{
	// Exit for counter clock-wise travel (case where the vertex neighborhood form a ring)
	bool hasCompletedFullCirculation = m_IsInCCWOrder && m_IsActive && m_CurFaceIdx == rhs.m_CurFaceIdx;
	// Exit for clock-wise travel (case where the vertex neighborhood is open and has a boundary)
	bool hasReachedBoundaryEnd = !m_IsInCCWOrder && m_CurFaceIdx == -1;
	return hasCompletedFullCirculation || hasReachedBoundaryEnd;
}

bool Mesh::FacesAroundVertexCirculator::operator!=(const Mesh::FacesAroundVertexCirculator& rhs) const
{
	return !operator==(rhs);
}

Mesh::FacesAroundVertexCirculator& Mesh::FacesAroundVertexCirculator::operator++()
{
	m_IsActive = true;

	int neighborFaceIdx;
	{
		const Face& curFace = m_Mesh.GetFaceData(m_CurFaceIdx);
		int localIdx = GetVertexLocalIndex(curFace, m_CentralVertexIdx);
		assert(localIdx != -1);
		neighborFaceIdx = curFace.Neighbors[IndexHelpers::Next[localIdx]];
	}

	if(m_IsInCCWOrder)
	{
		if(neighborFaceIdx == -1)
		{ // If there is no next face, we reached a boundary.
			while(m_JumpCount > 0)
			{ // We need to go back to the previous face and start going in the opposite direction.
				const Face& curFace = m_Mesh.GetFaceData(m_CurFaceIdx);
				int localIdx = GetVertexLocalIndex(curFace, m_CentralVertexIdx);
				assert(localIdx != -1);
				m_PrevFaceIdx = m_CurFaceIdx;
				m_CurFaceIdx = curFace.Neighbors[IndexHelpers::Previous[localIdx]];
				m_JumpCount--;
			}

			// We are now going in the clock-wise direction.
			m_IsInCCWOrder = false;
		}
		else // We are still going in the counter clock-wise direction.
		{
			// Update the jump count.
			m_JumpCount++;
			m_PrevFaceIdx = m_CurFaceIdx;
			m_CurFaceIdx = neighborFaceIdx;
		}
	}
	else // We are going in the clock-wise direction.
	{
		if(neighborFaceIdx == -1)
			return *this; // We reached a boundary, we stop here.

		m_PrevFaceIdx = m_CurFaceIdx;
		const Face& prevFace = m_Mesh.GetFaceData(m_CurFaceIdx);
		int localIdx = GetVertexLocalIndex(prevFace, m_CentralVertexIdx);
		assert(localIdx != -1);
		m_CurFaceIdx = prevFace.Neighbors[IndexHelpers::Previous[localIdx]];
	}

	return *this;
}

Mesh::FacesAroundVertexCirculator Mesh::FacesAroundVertexCirculator::operator++(int)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

FaceIndex Mesh::FacesAroundVertexCirculator::operator*() const
{
	return m_CurFaceIdx;
}

void Mesh::FacesAroundVertexCirculator::SetIsActive(bool value)
{
	m_IsActive = value;
}

//==========================FacesAroundVertexRange==========================//
Mesh::FacesAroundVertexRange::FacesAroundVertexRange(const Mesh& mesh, const VertexIndex index)
	: m_Mesh(mesh)
	, m_VertexIdx(index)
{}

Mesh::FacesAroundVertexCirculator Mesh::FacesAroundVertexRange::begin() const
{
	Mesh::FacesAroundVertexCirculator circ(m_Mesh, m_VertexIdx);
	circ.SetIsActive(false);
	return circ;
}

Mesh::FacesAroundVertexCirculator Mesh::FacesAroundVertexRange::end() const
{
	Mesh::FacesAroundVertexCirculator circ(m_Mesh, m_VertexIdx);
	circ.SetIsActive(true);
	return circ;
}

Mesh::FacesAroundVertexRange Mesh::GetFacesAroundVertex(const VertexIndex index) const
{
	return FacesAroundVertexRange(*this, index);
}

} // namespace Data::Surface
