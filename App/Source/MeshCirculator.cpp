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
	assert(curVertex.IncidentTriangleIdx != -1); // Its incident triangle index must be set.

	// Set the current triangle index.
	m_CurTriangleIdx = curVertex.IncidentTriangleIdx;

	// Get the current triangle data.
	const Triangle& curFace = m_Mesh.GetTriangleData(m_CurTriangleIdx);
	// Get the local index of the central mesh in the current triangle.
	int localIdx = GetVertexLocalIndex(curFace, m_CentralVertexIdx);
	assert(localIdx != -1); // The current triangle must contains the central vertex.

	//  Set the current vertex index.
	m_CurVertexLocalIdx = IndexHelpers::Previous[localIdx];
	m_CurVertexIdx = curFace.Vertices[m_CurVertexLocalIdx];
}

bool Mesh::VerticesAroundVertexCirculator::operator==(const Mesh::VerticesAroundVertexCirculator& rhs) const
{
	// Exit for counter clock-wise travel (case where the vertex neighborhood form a ring)
	bool hasCompletedFullCirculation = m_IsInCCWOrder && m_IsActive && m_CurVertexIdx == rhs.m_CurVertexIdx;
	// Exit for clock-wise travel (case where the vertex neighborhood is open and has a boundary)
	bool hasReachedBoundaryEnd = !m_IsInCCWOrder && m_CurTriangleIdx == -1;
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
			const Triangle& curFace = m_Mesh.GetTriangleData(m_CurTriangleIdx);
			neighborFaceIdx = curFace.Neighbors[IndexHelpers::Previous[m_CurVertexLocalIdx]];
		}
		if(neighborFaceIdx == -1)
		{ // If there is no next triangle, we reached a boundary.
			while(m_JumpCount > 0)
			{ // We need to go back to the previous triangle and start going in the opposite direction.
				const Triangle& curFace = m_Mesh.GetTriangleData(m_CurTriangleIdx);
				int localIdx = GetVertexLocalIndex(curFace, m_CentralVertexIdx);
				assert(localIdx != -1);
				m_PrevTriangleIdx = m_CurTriangleIdx;
				m_CurTriangleIdx = curFace.Neighbors[IndexHelpers::Previous[localIdx]];
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
			m_PrevTriangleIdx = m_CurTriangleIdx;
			m_CurTriangleIdx = neighborFaceIdx;

			UpdateCurVertexIndexInCCWOrder();
		}
	}
	else // We are going in the clock-wise direction.
	{
		m_PrevTriangleIdx = m_CurTriangleIdx;
		const Triangle& prevFace = m_Mesh.GetTriangleData(m_CurTriangleIdx);
		m_CurTriangleIdx = prevFace.Neighbors[IndexHelpers::Next[m_CurVertexLocalIdx]];

		if(m_CurTriangleIdx == -1)
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
	const Triangle& curFace = m_Mesh.GetTriangleData(m_CurTriangleIdx);
	int localIdx = GetVertexLocalIndex(curFace, m_CentralVertexIdx);
	assert(localIdx != -1);

	// Update the vertex and triangle informations.
	m_CurVertexLocalIdx = IndexHelpers::Next[localIdx];
	m_CurVertexIdx = curFace.Vertices[m_CurVertexLocalIdx];
}

void Mesh::VerticesAroundVertexCirculator::UpdateCurVertexIndexInCCWOrder()
{
	const Triangle& curFace = m_Mesh.GetTriangleData(m_CurTriangleIdx);
	int localIdx = GetVertexLocalIndex(curFace, m_CentralVertexIdx);
	assert(localIdx != -1);

	// Update the vertex and triangle informations.
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

//==========================TrianglesAroundVertexCirculator==========================//
Mesh::TrianglesAroundVertexCirculator::TrianglesAroundVertexCirculator(const Mesh& mesh, const VertexIndex index)
	: m_Mesh(mesh)
	, m_CentralVertexIdx(index)
{
	// Get the vertex data of the central vertex.
	const Vertex& curVertex = m_Mesh.GetVertexData(m_CentralVertexIdx);
	assert(curVertex.IncidentTriangleIdx != -1); // Its incident triangle index must be set.

	// Set the current triangle index.
	m_CurTriangleIdx = curVertex.IncidentTriangleIdx;
	m_PrevTriangleIdx = -1;
	m_JumpCount++;
}

bool Mesh::TrianglesAroundVertexCirculator::operator==(const Mesh::TrianglesAroundVertexCirculator& rhs) const
{
	// Exit for counter clock-wise travel (case where the vertex neighborhood form a ring)
	bool hasCompletedFullCirculation = m_IsInCCWOrder && m_IsActive && m_CurTriangleIdx == rhs.m_CurTriangleIdx;
	// Exit for clock-wise travel (case where the vertex neighborhood is open and has a boundary)
	bool hasReachedBoundaryEnd = !m_IsInCCWOrder && m_CurTriangleIdx == -1;
	return hasCompletedFullCirculation || hasReachedBoundaryEnd;
}

bool Mesh::TrianglesAroundVertexCirculator::operator!=(const Mesh::TrianglesAroundVertexCirculator& rhs) const
{
	return !operator==(rhs);
}

Mesh::TrianglesAroundVertexCirculator& Mesh::TrianglesAroundVertexCirculator::operator++()
{
	m_IsActive = true;

	int neighborFaceIdx;
	{
		const Triangle& curFace = m_Mesh.GetTriangleData(m_CurTriangleIdx);
		int localIdx = GetVertexLocalIndex(curFace, m_CentralVertexIdx);
		assert(localIdx != -1);
		neighborFaceIdx = curFace.Neighbors[IndexHelpers::Next[localIdx]];
	}

	if(m_IsInCCWOrder)
	{
		if(neighborFaceIdx == -1)
		{ // If there is no next triangle, we reached a boundary.
			while(m_JumpCount > 0)
			{ // We need to go back to the previous triangle and start going in the opposite direction.
				const Triangle& curFace = m_Mesh.GetTriangleData(m_CurTriangleIdx);
				int localIdx = GetVertexLocalIndex(curFace, m_CentralVertexIdx);
				assert(localIdx != -1);
				m_PrevTriangleIdx = m_CurTriangleIdx;
				m_CurTriangleIdx = curFace.Neighbors[IndexHelpers::Previous[localIdx]];
				m_JumpCount--;
			}

			// We are now going in the clock-wise direction.
			m_IsInCCWOrder = false;
		}
		else // We are still going in the counter clock-wise direction.
		{
			// Update the jump count.
			m_JumpCount++;
			m_PrevTriangleIdx = m_CurTriangleIdx;
			m_CurTriangleIdx = neighborFaceIdx;
		}
	}
	else // We are going in the clock-wise direction.
	{
		m_PrevTriangleIdx = m_CurTriangleIdx;
		const Triangle& prevFace = m_Mesh.GetTriangleData(m_CurTriangleIdx);
		int localIdx = GetVertexLocalIndex(prevFace, m_CentralVertexIdx);
		assert(localIdx != -1);
		m_CurTriangleIdx = prevFace.Neighbors[IndexHelpers::Previous[localIdx]];
	}

	return *this;
}

TriangleIndex Mesh::TrianglesAroundVertexCirculator::operator*() const
{
	return m_CurTriangleIdx;
}

void Mesh::TrianglesAroundVertexCirculator::SetIsActive(bool value)
{
	m_IsActive = value;
}

//==========================TrianglesAroundVertexRange==========================//
Mesh::TrianglesAroundVertexRange::TrianglesAroundVertexRange(const Mesh& mesh, const VertexIndex index)
	: m_Mesh(mesh)
	, m_VertexIdx(index)
{}

Mesh::TrianglesAroundVertexCirculator Mesh::TrianglesAroundVertexRange::begin() const
{
	Mesh::TrianglesAroundVertexCirculator circ(m_Mesh, m_VertexIdx);
	circ.SetIsActive(false);
	return circ;
}

Mesh::TrianglesAroundVertexCirculator Mesh::TrianglesAroundVertexRange::end() const
{
	Mesh::TrianglesAroundVertexCirculator circ(m_Mesh, m_VertexIdx);
	circ.SetIsActive(true);
	return circ;
}

Mesh::TrianglesAroundVertexRange Mesh::GetTrianglesAroundVertex(const VertexIndex index) const
{
	return TrianglesAroundVertexRange(*this, index);
}
} // namespace Data::Surface
