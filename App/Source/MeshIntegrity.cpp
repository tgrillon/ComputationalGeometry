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

		// Check that the vertex is indeed part of its incident face.
		const Face& face = mesh.m_Faces[curVertex.IncidentFaceIdx];
		auto&& faceVertices = face.Vertices;
		if(faceVertices[0] != vIdx && faceVertices[1] != vIdx && faceVertices[2] != vIdx)
			return Status::VertexNotInFace;
	}

	// Check the integrity of each face of the mesh.
	for(auto&& curFace : mesh.m_Faces)
	{
		// Check that the face has valid vertices.
		if(curFace.Vertices[0] == -1 || curFace.Vertices[1] == -1 || curFace.Vertices[2] == -1)
			return Status::FaceHasNullVertex;

		// Check that each neighbor face is reciprocal.
		for(uint8_t idx = 0; idx < 3; ++idx)
		{
			IndexType neighborIdx = curFace.Neighbors[idx];
			if(neighborIdx != -1)
			{
				// Get the two vertices defining the edge opposite to the neighbor.
				IndexType v0Idx = curFace.Vertices[IndexHelpers::Next[idx]];
				IndexType v1Idx = curFace.Vertices[IndexHelpers::Previous[idx]];

				// Get the neighbor face and the two vertices.
				const Face& neighbor = mesh.m_Faces[neighborIdx];
				const Vertex& v0 = mesh.m_Vertices[v0Idx];
				const Vertex& v1 = mesh.m_Vertices[v1Idx];
				int8_t edgeIndex = Utilitary::Primitive::GetEdgeIndex(neighbor, v0, v1);

				// Check that the neighbor has the current face as neighbor on the same edge.
				if(edgeIndex == -1 || neighbor.Neighbors[edgeIndex] != curFace.Index)
					return Status::FaceNeighborNotReciprocal;
			}
		}
	}
	return MeshIntegrity::Status::MeshOK;
}
} // namespace Utilitary::Mesh
