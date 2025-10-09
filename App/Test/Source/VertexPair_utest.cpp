#include "Application/VertexPair.h"

#include <gtest/gtest.h>

TEST(VertexPairTest, Creation_ValidVertices_ShouldCreatePair)
{
	Data::Primitive::Vertex v1{ .Index = 1 };
	Data::Primitive::Vertex v2{ .Index = 2 };

	Data::Primitive::VertexPair pair(v1, v2);

	EXPECT_EQ(pair.GetMinVertexIndex(), 1);
	EXPECT_EQ(pair.GetMaxVertexIndex(), 2);
}

TEST(VertexPairTest, Creation_IdenticalVertices_ShouldThrow)
{
	Data::Primitive::Vertex v1{ .Index = 1 };
	Data::Primitive::Vertex v2{ .Index = 1 };

	EXPECT_THROW(Data::Primitive::VertexPair(v1, v2), std::invalid_argument);
}
