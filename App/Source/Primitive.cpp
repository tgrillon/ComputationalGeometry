#include "Application/Primitive.h"

using namespace Data::Primitive;
using namespace BaseType;

namespace Utilitary::Primitive
{
int8_t GetEdgeIndex(const Face& face, const Vertex& v0, const Vertex& v1)
{
	// Initialize edge index to -1 (not found)
	int8_t edgeIdx = -1;

	// Lambda to find the edge index given a vertex and the current index
	auto FindEdgeIndex = [&](const Vertex& vertex, const uint8_t idx)
	{
		IndexType prevVertexIdx = face.Vertices[IndexHelpers::Previous[idx]];
		IndexType nextVertexIdx = face.Vertices[IndexHelpers::Next[idx]];
		if(prevVertexIdx == vertex.Index)
			return static_cast<int8_t>(IndexHelpers::Next[idx]);
		else if(nextVertexIdx == vertex.Index)
			return static_cast<int8_t>(IndexHelpers::Previous[idx]);

		return static_cast<int8_t>(-1);
	};

	// Iterate through the vertices of the face to find the edge
	for(uint8_t idx = 0; idx < 3; ++idx)
	{
		IndexType curVertexIdx = face.Vertices[IndexHelpers::Current[idx]];
		if(curVertexIdx == v0.Index)
			edgeIdx = FindEdgeIndex(v1, idx);
		else if(curVertexIdx == v1.Index)
			edgeIdx = FindEdgeIndex(v0, idx);
	}

	return edgeIdx;
}
} // namespace Utilitary::Primitive
