#include "Application/Primitive.h"

#include <gtest/gtest.h>

using namespace Utilitary::Primitive;
using namespace Data::Primitive;

TEST(PrimitiveTest, GetEdgeIndex_ValidEdge_ShouldReturnCorrectIndex)
{
	Face face{ .Vertices = { 0, 1, 2 } };

	EXPECT_EQ(GetEdgeIndex(face, 0, 1), 2);
	EXPECT_EQ(GetEdgeIndex(face, 1, 2), 0);
	EXPECT_EQ(GetEdgeIndex(face, 2, 0), 1);
	EXPECT_EQ(GetEdgeIndex(face, 1, 0), 2);
	EXPECT_EQ(GetEdgeIndex(face, 2, 1), 0);
	EXPECT_EQ(GetEdgeIndex(face, 0, 2), 1);
}

TEST(PrimitiveTest, GetEdgeIndex_InvalidEdge_ShouldReturnMinusOne)
{
	Face face{ .Vertices = { 0, 1, 2 } };

	EXPECT_EQ(GetEdgeIndex(face, 0, 3), -1);
	EXPECT_EQ(GetEdgeIndex(face, 3, 0), -1);
	EXPECT_EQ(GetEdgeIndex(face, 1, 3), -1);
	EXPECT_EQ(GetEdgeIndex(face, 3, 1), -1);
	EXPECT_EQ(GetEdgeIndex(face, 2, 3), -1);
	EXPECT_EQ(GetEdgeIndex(face, 3, 2), -1);
	EXPECT_EQ(GetEdgeIndex(face, 3, 4), -1);
}

TEST(PrimitiveTest, GetVertexLocalIndex_ValidVertex_ShouldReturnCorrectIndex)
{
	{
		Face face{ .Vertices = { 0, 1, 2 } };

		EXPECT_EQ(GetVertexLocalIndex(face, 0), 0);
		EXPECT_EQ(GetVertexLocalIndex(face, 1), 1);
		EXPECT_EQ(GetVertexLocalIndex(face, 2), 2);
	}

	{
		Face face{ .Vertices = { 5, 8, 1 } };

		EXPECT_EQ(GetVertexLocalIndex(face, 5), 0);
		EXPECT_EQ(GetVertexLocalIndex(face, 8), 1);
		EXPECT_EQ(GetVertexLocalIndex(face, 1), 2);
	}
}

TEST(PrimitiveTest, GetVertexLocalIndex_InvalidVertex_ShouldReturnMinusOne)
{
	{
		Face face{ .Vertices = { 0, 1, 2 } };

		EXPECT_EQ(GetVertexLocalIndex(face, 4), -1);
		EXPECT_EQ(GetVertexLocalIndex(face, 8), -1);
		EXPECT_EQ(GetVertexLocalIndex(face, 6), -1);
	}
}
