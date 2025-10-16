#include "Application/MeshIntegrity.h"

using namespace BaseType;
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
		// Check that the vertex has a valid incident face.
		if(curVertex.IncidentFaceIdx == -1)
			return ExitCode::VertexHasNullIncidentFace;

		// Check that the vertex has a valid incident face index.
		if(static_cast<uint32_t>(curVertex.IncidentFaceIdx) >= mesh.GetFaceCount())
			return ExitCode::InvalidIncidentFaceIndex;

		// Check that the vertex is indeed part of its incident face.
		const Face& face = mesh.m_Faces[curVertex.IncidentFaceIdx];
		auto&& faceVertices = face.Vertices;
		if(faceVertices[0] != iVertex && faceVertices[1] != iVertex && faceVertices[2] != iVertex)
			return ExitCode::VertexNotInFace;
	}

	// Check the integrity of each face of the mesh.
	int vertexCount = static_cast<int>(mesh.GetVertexCount());
	int faceCount = static_cast<int>(mesh.GetFaceCount());
	for(int iFace = 0; iFace < faceCount; ++iFace)
	{
		const Face& curFace = mesh.m_Faces[iFace];
		// Check that the face has valid vertices.
		if(curFace.Vertices[0] == -1 || curFace.Vertices[1] == -1 || curFace.Vertices[2] == -1)
			return ExitCode::FaceHasNullVertex;

		// Check that the face does not have duplicated vertices.
		if(curFace.Vertices[0] == curFace.Vertices[1] || curFace.Vertices[1] == curFace.Vertices[2]
		   || curFace.Vertices[2] == curFace.Vertices[0])
			return ExitCode::FaceHasDuplicatedVertices;

		for(EdgeIndex iEdge = 0; iEdge < 3; ++iEdge)
		{
			// Check that each vertex has a valid index.
			if(curFace.Vertices[iEdge] >= vertexCount)
				return MeshIntegrity::ExitCode::InvalidVertexIndex;

			// Check that each neighbor face is reciprocal.
			int neighborIdx = curFace.Neighbors[iEdge];
			if(neighborIdx != -1)
			{
				// Get the two vertices defining the edge opposite to the neighbor.
				VertexIndex v0Idx = curFace.Vertices[IndexHelpers::Next[iEdge]];
				VertexIndex v1Idx = curFace.Vertices[IndexHelpers::Previous[iEdge]];

				// Check that the neighbor index is valid.
				if(neighborIdx >= faceCount)
					return ExitCode::InvalidNeighborFaceIndex;

				// A face cannot be its own neighbor.
				if(neighborIdx == iFace)
					return ExitCode::FaceIsItsOwnNeighbor;

				// Get the neighbor face and the two vertices.
				const Face& neighbor = mesh.m_Faces[neighborIdx];
				int edgeIndex = Utilitary::Primitive::GetEdgeIndex(neighbor, v0Idx, v1Idx);

				// Check that the neighbor has the current face as neighbor on the same edge.
				if(edgeIndex == -1 || neighbor.Neighbors[edgeIndex] != iFace)
					return ExitCode::FaceNeighborNotReciprocal;
			}
		}
	}
	return MeshIntegrity::ExitCode::MeshOK;
}
} // namespace Utilitary::Surface
