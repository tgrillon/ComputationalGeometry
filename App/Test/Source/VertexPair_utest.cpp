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

TEST(VertexPairTest, Creation_IdenticalVertexPairs_ShouldBeEqual)
{
	Data::Primitive::Vertex A{ .Index = 1 };
	Data::Primitive::Vertex B{ .Index = 2 };

	Data::Primitive::VertexPair AB(A, B);
	EXPECT_EQ(AB.GetMinVertexIndex(), 1);
	EXPECT_EQ(AB.GetMaxVertexIndex(), 2);
	Data::Primitive::VertexPair BA(B, A);
	EXPECT_EQ(BA.GetMinVertexIndex(), 1);
	EXPECT_EQ(BA.GetMaxVertexIndex(), 2);

	// Check hash validity.
	std::unordered_map<Data::Primitive::VertexPair, bool> map;
	map[AB] = true;
	EXPECT_NE(map.find(BA), map.end());
}

TEST(VertexPairTest, Creation_IdenticalVertices_ShouldThrow)
{
	Data::Primitive::Vertex v1{ .Index = 1 };
	Data::Primitive::Vertex v2{ .Index = 1 };

	EXPECT_THROW(Data::Primitive::VertexPair(v1, v2), std::invalid_argument);
}
