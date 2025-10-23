#include "Application/MeshIntegrity.h"

using namespace Core::BaseType;
using namespace Data::Primitive;
using namespace Data::Surface;

namespace Utilitary::Surface
{
MeshIntegrity::ExitCode MeshIntegrity::CheckIntegrity(const Mesh& mesh)
{
	// Check the integrity of each vertex of the mesh.
	for(int iVertex = 0; iVertex < static_cast<int>(mesh.GetVertexCount()); ++iVertex)
	{
		const Vertex& curVertex = mesh.m_Vertices[iVertex];
		// Check that the vertex has a valid incident triangle.
		if(curVertex.IncidentTriangleIdx == -1)
			return ExitCode::VertexHasNullIncidentTriangle;

		// Check that the vertex has a valid incident triangle index.
		if(static_cast<uint32_t>(curVertex.IncidentTriangleIdx) >= mesh.GetTriangleCount())
			return ExitCode::InvalidIncidentTriangleIndex;

		// Check that the vertex is indeed part of its incident triangle.
		const Triangle& triangle = mesh.m_Triangles[curVertex.IncidentTriangleIdx];
		auto&& triangleVertices = triangle.Vertices;
		if(triangleVertices[0] != iVertex && triangleVertices[1] != iVertex && triangleVertices[2] != iVertex)
			return ExitCode::VertexNotInTriangle;
	}

	// Check the integrity of each triangle of the mesh.
	int vertexCount = static_cast<int>(mesh.GetVertexCount());
	int triangleCount = static_cast<int>(mesh.GetTriangleCount());
	for(int iTriangle = 0; iTriangle < triangleCount; ++iTriangle)
	{
		const Triangle& curTriangle = mesh.m_Triangles[iTriangle];
		// Check that the triangle has valid vertices.
		if(curTriangle.Vertices[0] == -1 || curTriangle.Vertices[1] == -1 || curTriangle.Vertices[2] == -1)
			return ExitCode::TriangleHasNullVertex;

		// Check that the triangle does not have duplicated vertices.
		if(curTriangle.Vertices[0] == curTriangle.Vertices[1] || curTriangle.Vertices[1] == curTriangle.Vertices[2]
		   || curTriangle.Vertices[2] == curTriangle.Vertices[0])
			return ExitCode::TriangleHasDuplicatedVertices;

		for(EdgeIndex iEdge = 0; iEdge < 3; ++iEdge)
		{
			// Check that each vertex has a valid index.
			if(curTriangle.Vertices[iEdge] >= vertexCount)
				return MeshIntegrity::ExitCode::InvalidVertexIndex;

			// Check that each neighbor triangle is reciprocal.
			int neighborIdx = curTriangle.Neighbors[iEdge];
			if(neighborIdx != -1)
			{
				// Get the two vertices defining the edge opposite to the neighbor.
				VertexIndex v0Idx = curTriangle.Vertices[IndexHelpers::Next[iEdge]];
				VertexIndex v1Idx = curTriangle.Vertices[IndexHelpers::Previous[iEdge]];

				// Check that the neighbor index is valid.
				if(neighborIdx >= triangleCount)
					return ExitCode::InvalidNeighborTriangleIndex;

				// A triangle cannot be its own neighbor.
				if(neighborIdx == iTriangle)
					return ExitCode::TriangleIsItsOwnNeighbor;

				// Get the neighbor triangle and the two vertices.
				const Triangle& neighbor = mesh.m_Triangles[neighborIdx];
				int edgeIndex = Utilitary::Primitive::GetEdgeIndex(neighbor, v0Idx, v1Idx);

				// Check that the neighbor has the current triangle as neighbor on the same edge.
				if(edgeIndex == -1 || neighbor.Neighbors[edgeIndex] != iTriangle)
					return ExitCode::TriangleNeighborNotReciprocal;
			}
		}
	}
	return MeshIntegrity::ExitCode::MeshOK;
}
} // namespace Utilitary::Surface
