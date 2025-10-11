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
	VertexPair(const BaseType::IndexType v0Idx, const BaseType::IndexType v1Idx)
	{
		if(v0Idx < v1Idx)
		{
			m_MinVertexIdx = v0Idx;
			m_MaxVertexIdx = v1Idx;
		}
		else if(v0Idx > v1Idx)
		{
			m_MinVertexIdx = v1Idx;
			m_MaxVertexIdx = v0Idx;
		}
		else
		{
			throw std::invalid_argument("VertexPair cannot be created with two identical vertex indices.");
		}
	}

	bool operator==(const VertexPair& other) const
	{
		return (m_MinVertexIdx == other.m_MinVertexIdx && m_MaxVertexIdx == other.m_MaxVertexIdx)
			|| (m_MinVertexIdx == other.m_MaxVertexIdx && m_MaxVertexIdx == other.m_MinVertexIdx);
	}

	~VertexPair() = default;

	/// @brief Get the index of the minimum vertex.
	uint32_t GetMinVertexIdx() const { return m_MinVertexIdx; }

	/// @brief Get the index of the maximum vertex.
	uint32_t GetMaxVertexIdx() const { return m_MaxVertexIdx; }

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
