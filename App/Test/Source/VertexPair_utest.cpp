#include "Application/VertexPair.h"

#include <gtest/gtest.h>

TEST(VertexPairTest, Creation_ValidVertices_ShouldCreatePair)
{
	Data::Primitive::VertexPair pair(1, 2);

	EXPECT_EQ(pair.GetMinVertexIdx(), 1);
	EXPECT_EQ(pair.GetMaxVertexIdx(), 2);
}

TEST(VertexPairTest, Creation_IdenticalVertexPairs_ShouldBeEqual)
{
	Core::BaseType::VertexIndex idxA{ 1 };
	Core::BaseType::VertexIndex idxB{ 2 };

	Data::Primitive::VertexPair AB(idxA, idxB);
	EXPECT_EQ(AB.GetMinVertexIdx(), 1);
	EXPECT_EQ(AB.GetMaxVertexIdx(), 2);
	Data::Primitive::VertexPair BA(idxB, idxA);
	EXPECT_EQ(BA.GetMinVertexIdx(), 1);
	EXPECT_EQ(BA.GetMaxVertexIdx(), 2);

	// Check hash validity.
	std::unordered_map<Data::Primitive::VertexPair, bool> map;
	map[AB] = true;
	EXPECT_NE(map.find(BA), map.end());
}

TEST(VertexPairTest, Creation_IdenticalVertices_ShouldThrow)
{
	EXPECT_THROW(Data::Primitive::VertexPair(1, 1), std::invalid_argument);
}
