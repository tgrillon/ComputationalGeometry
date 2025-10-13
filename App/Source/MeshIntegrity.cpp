#include "Application/MeshIntegrity.h"

using namespace BaseType;
using namespace Data::Primitive;

namespace Utilitary::Mesh
{
MeshIntegrity::Status MeshIntegrity::CheckIntegrity(const Data::Surface::Mesh& mesh)
{
	// Check the integrity of each vertex of the mesh.
	for(IndexType vIdx = 0; vIdx < mesh.GetVertexCount(); ++vIdx)
	{
		const Vertex& curVertex = mesh.m_Vertices[vIdx];
		// Check that the vertex has a valid incident face.
		if(curVertex.IncidentFaceIdx == -1)
			return Status::VertexHasNullIncidentFace;

		// Check that the vertex has a valid incident face index.
		if(curVertex.IncidentFaceIdx >= mesh.GetFaceCount())
			return Status::InvalidIncidentFaceIndex;

		// Check that the vertex is indeed part of its incident face.
		const Face& face = mesh.m_Faces[curVertex.IncidentFaceIdx];
		auto&& faceVertices = face.Vertices;
		if(faceVertices[0] != vIdx && faceVertices[1] != vIdx && faceVertices[2] != vIdx)
			return Status::VertexNotInFace;
	}

	// Check the integrity of each face of the mesh.
	for(IndexType curFaceIdx = 0; curFaceIdx < mesh.GetFaceCount(); ++curFaceIdx)
	{
		const Face& curFace = mesh.m_Faces[curFaceIdx];
		// Check that the face has valid vertices.
		if(curFace.Vertices[0] == -1 || curFace.Vertices[1] == -1 || curFace.Vertices[2] == -1)
			return Status::FaceHasNullVertex;

		// Check that the face does not have duplicated vertices.
		if(curFace.Vertices[0] == curFace.Vertices[1] || curFace.Vertices[1] == curFace.Vertices[2]
		   || curFace.Vertices[2] == curFace.Vertices[0])
			return Status::FaceHasDuplicatedVertices;

		for(uint8_t curEdgeIdx = 0; curEdgeIdx < 3; ++curEdgeIdx)
		{
			// Check that each vertex has a valid index.
			if(curFace.Vertices[curEdgeIdx] >= mesh.GetVertexCount())
				return MeshIntegrity::Status::InvalidVertexIndex;

			// Check that each neighbor face is reciprocal.
			IndexType neighborIdx = curFace.Neighbors[curEdgeIdx];
			if(neighborIdx != -1)
			{
				// Get the two vertices defining the edge opposite to the neighbor.
				IndexType v0Idx = curFace.Vertices[IndexHelpers::Next[curEdgeIdx]];
				IndexType v1Idx = curFace.Vertices[IndexHelpers::Previous[curEdgeIdx]];

				// Check that the neighbor index is valid.
				if(neighborIdx >= mesh.GetFaceCount())
					return Status::InvalidNeighborFaceIndex;

				// A face cannot be its own neighbor.
				if(neighborIdx == curFaceIdx)
					return Status::FaceIsItsOwnNeighbor;

				// Get the neighbor face and the two vertices.
				const Face& neighbor = mesh.m_Faces[neighborIdx];
				int8_t edgeIndex = Utilitary::Primitive::GetEdgeIndex(neighbor, v0Idx, v1Idx);

				// Check that the neighbor has the current face as neighbor on the same edge.
				if(edgeIndex == -1 || neighbor.Neighbors[edgeIndex] != curFaceIdx)
					return Status::FaceNeighborNotReciprocal;
			}
		}
	}
	return MeshIntegrity::Status::MeshOK;
}
} // namespace Utilitary::Mesh
