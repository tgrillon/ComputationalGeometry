#include "Application/MeshIntegrity.h"

using namespace BaseType;
using namespace Data::Primitive;

namespace Utilitary::Mesh
{
MeshIntegrity::Status MeshIntegrity::CheckIntegrity(const Data::Surface::Mesh& mesh)
{
	// Check the integrity of each vertex of the mesh.
	for(auto&& curVertex : mesh.m_Vertices)
	{
		// Check that the vertex has a valid incident face.
		Face* face = curVertex.IncidentFace;
		if(face == nullptr)
			return Status::VertexHasNullIncidentFace;

		// Check that the vertex is indeed part of its incident face.
		auto&& faceVertices = face->Vertices;
		if(faceVertices[0] != &curVertex && faceVertices[1] != &curVertex && faceVertices[2] != &curVertex)
			return Status::VertexNotInFace;
	}

	// Check the integrity of each face of the mesh.
	for(auto&& curFace : mesh.m_Faces)
	{
		// Check that the face has valid vertices.
		if(curFace.Vertices[0] == nullptr || curFace.Vertices[1] == nullptr || curFace.Vertices[2] == nullptr)
			return Status::FaceHasNullVertex;

		// Check that each neighbor face is reciprocal.
		for(uint8_t idx = 0; idx < 3; ++idx)
		{
			Face* neighbor = curFace.Neighbors[idx];
			if(neighbor != nullptr)
			{
				// Get the two vertices defining the edge shared with the neighbor.
				Vertex& v0 = *curFace.Vertices[IndexHelpers::Next[idx]];
				Vertex& v1 = *curFace.Vertices[IndexHelpers::Previous[idx]];
				int8_t edgeIndex = Utilitary::Primitive::GetEdgeIndex(*neighbor, v0, v1);

				// Check that the neighbor has the current face as neighbor on the same edge.
				if(edgeIndex == -1 || neighbor->Neighbors[edgeIndex] != &curFace)
					return Status::FaceNeighborNotReciprocal;
			}
		}
	}
	return MeshIntegrity::Status::MeshOK;
}
} // namespace Utilitary::Mesh
