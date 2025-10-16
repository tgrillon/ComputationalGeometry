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
	VertexPair(const BaseType::VertexIndex firstIndex, const BaseType::VertexIndex secondIndex);

	/// @brief Equality operator.
	bool operator==(const VertexPair& other) const;

	/// @brief Default destructor.
	~VertexPair() = default;

	/// @brief Get the index of the minimum vertex.
	uint32_t GetMinVertexIdx() const;

	/// @brief Get the index of the maximum vertex.
	uint32_t GetMaxVertexIdx() const;

private:
	/// @brief Minimum vertex (the one with the smaller index).
	BaseType::VertexIndex m_MinVertexIdx;
	/// @brief Maximum vertex (the one with the biggest index).
	BaseType::VertexIndex m_MaxVertexIdx;
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
		auto h1 = std::hash<BaseType::VertexIndex>{}(vertexPair.GetMinVertexIdx());
		auto h2 = std::hash<BaseType::VertexIndex>{}(vertexPair.GetMaxVertexIdx());
		return h1 ^ h2;
	}
};
} // namespace std
