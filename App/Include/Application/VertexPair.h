#pragma once

#include "Application/Primitive.h"

#include <bitset>
#include <stdexcept>

namespace Data::Primitive
{
/// @brief Class representing a pair of vertices.
/// @note The order of the vertices does not matter (i.e., (v1, v2) is the same as (v2, v1)).
class VertexPair
{
public:
	/// @brief Construct a VertexPair from two vertices.
	VertexPair(const BaseType::IndexType v0Idx, const BaseType::IndexType v1Idx);

	bool operator==(const VertexPair& other) const;

	~VertexPair() = default;

	/// @brief Get the index of the minimum vertex.
	uint32_t GetMinVertexIdx() const;

	/// @brief Get the index of the maximum vertex.
	uint32_t GetMaxVertexIdx() const;

private:
	/// @brief Minimum vertex (the one with the smaller index).
	BaseType::IndexType m_MinVertexIdx;
	/// @brief Maximum vertex (the one with the biggest index).
	BaseType::IndexType m_MaxVertexIdx;
};
} // namespace Data::Primitive

/// Hash function for VertexPair to be used in unordered containers.
namespace std
{
template<>
struct hash<Data::Primitive::VertexPair>
{
	size_t operator()(const Data::Primitive::VertexPair& vertexPair) const
	{
		auto h1 = std::hash<BaseType::IndexType>{}(vertexPair.GetMinVertexIdx());
		auto h2 = std::hash<BaseType::IndexType>{}(vertexPair.GetMaxVertexIdx());
		return h1 ^ h2;
	}
};
} // namespace std
