#include "Application/Mesh.h"

using namespace Data::Primitive;
using namespace Utilitary::Primitive;
using namespace BaseType;

namespace Data::Surface
{
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

	if(m_IsInCCWOrder)
	{
		int neighborFaceIdx;
		{
			const Face& curFace = m_Mesh.GetFaceData(m_CurFaceIdx);
			neighborFaceIdx = curFace.Neighbors[IndexHelpers::Previous[m_CurVertexLocalIdx]];
		}
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
		m_PrevFaceIdx = m_CurFaceIdx;
		const Face& prevFace = m_Mesh.GetFaceData(m_CurFaceIdx);
		int localIdx = GetVertexLocalIndex(prevFace, m_CentralVertexIdx);
		assert(localIdx != -1);
		m_CurFaceIdx = prevFace.Neighbors[IndexHelpers::Previous[localIdx]];
	}

	return *this;
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
