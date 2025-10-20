#include "Application/Primitive.h"

#include <gtest/gtest.h>

using namespace Utilitary::Primitive;
using namespace Data::Primitive;

TEST(PrimitiveTest, GetEdgeIndex_ValidEdge_ShouldReturnCorrectIndex)
{
	Triangle triangle{ .Vertices = { 0, 1, 2 } };

	EXPECT_EQ(GetEdgeIndex(triangle, 0, 1), 2);
	EXPECT_EQ(GetEdgeIndex(triangle, 1, 2), 0);
	EXPECT_EQ(GetEdgeIndex(triangle, 2, 0), 1);
	EXPECT_EQ(GetEdgeIndex(triangle, 1, 0), 2);
	EXPECT_EQ(GetEdgeIndex(triangle, 2, 1), 0);
	EXPECT_EQ(GetEdgeIndex(triangle, 0, 2), 1);
}

TEST(PrimitiveTest, GetEdgeIndex_InvalidEdge_ShouldReturnMinusOne)
{
	Triangle triangle{ .Vertices = { 0, 1, 2 } };

	EXPECT_EQ(GetEdgeIndex(triangle, 0, 3), -1);
	EXPECT_EQ(GetEdgeIndex(triangle, 3, 0), -1);
	EXPECT_EQ(GetEdgeIndex(triangle, 1, 3), -1);
	EXPECT_EQ(GetEdgeIndex(triangle, 3, 1), -1);
	EXPECT_EQ(GetEdgeIndex(triangle, 2, 3), -1);
	EXPECT_EQ(GetEdgeIndex(triangle, 3, 2), -1);
	EXPECT_EQ(GetEdgeIndex(triangle, 3, 4), -1);
}

TEST(PrimitiveTest, GetVertexLocalIndex_ValidVertex_ShouldReturnCorrectIndex)
{
	{
		Triangle triangle{ .Vertices = { 0, 1, 2 } };

		EXPECT_EQ(GetVertexLocalIndex(triangle, 0), 0);
		EXPECT_EQ(GetVertexLocalIndex(triangle, 1), 1);
		EXPECT_EQ(GetVertexLocalIndex(triangle, 2), 2);
	}

	{
		Triangle triangle{ .Vertices = { 5, 8, 1 } };

		EXPECT_EQ(GetVertexLocalIndex(triangle, 5), 0);
		EXPECT_EQ(GetVertexLocalIndex(triangle, 8), 1);
		EXPECT_EQ(GetVertexLocalIndex(triangle, 1), 2);
	}
}

TEST(PrimitiveTest, GetVertexLocalIndex_InvalidVertex_ShouldReturnMinusOne)
{
	{
		Triangle triangle{ .Vertices = { 0, 1, 2 } };

		EXPECT_EQ(GetVertexLocalIndex(triangle, 4), -1);
		EXPECT_EQ(GetVertexLocalIndex(triangle, 8), -1);
		EXPECT_EQ(GetVertexLocalIndex(triangle, 6), -1);
	}
}
