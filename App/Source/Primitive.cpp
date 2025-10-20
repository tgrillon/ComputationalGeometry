#include "Application/Primitive.h"

using namespace Data::Primitive;
using namespace BaseType;

namespace Utilitary::Primitive
{
int GetEdgeIndex(const Triangle& triangle, const VertexIndex firstIndex, const VertexIndex secondIndex)
{
	// Initialize edge index to -1 (not found)
	int edgeIdx = -1;

	// Lambda to find the edge index given a vertex and the current index
	auto FindEdgeIndex = [&](const VertexIndex vertexIdx, const uint8_t idx)
	{
		VertexIndex prevVertexIdx = triangle.Vertices[IndexHelpers::Previous[idx]];
		VertexIndex nextVertexIdx = triangle.Vertices[IndexHelpers::Next[idx]];
		if(prevVertexIdx == vertexIdx)
			return static_cast<int>(IndexHelpers::Next[idx]);
		else if(nextVertexIdx == vertexIdx)
			return static_cast<int>(IndexHelpers::Previous[idx]);

		return -1;
	};

	// Iterate through the vertices of the triangle to find the edge
	for(EdgeIndex iEdge = 0; iEdge < 3; ++iEdge)
	{
		VertexIndex curVertexIdx = triangle.Vertices[IndexHelpers::Current[iEdge]];
		if(curVertexIdx == firstIndex)
			edgeIdx = FindEdgeIndex(secondIndex, iEdge);
		else if(curVertexIdx == secondIndex)
			edgeIdx = FindEdgeIndex(firstIndex, iEdge);
	}

	return edgeIdx;
}

int GetVertexLocalIndex(const Triangle& triangle, const VertexIndex index)
{
	for(int iEdge = 0; iEdge < 3; ++iEdge)
		if(static_cast<VertexIndex>(triangle.Vertices[iEdge]) == index)
			return iEdge;

	return -1;
}
} // namespace Utilitary::Primitive
