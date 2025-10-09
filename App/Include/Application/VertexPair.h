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
	VertexPair(const Vertex& v0, const Vertex& v1)
	{
		if(v0.Index < v1.Index)
		{
			m_MinVertex = const_cast<Vertex*>(&v0);
			m_MaxVertex = const_cast<Vertex*>(&v1);
		}
		else if(v0.Index > v1.Index)
		{
			m_MinVertex = const_cast<Vertex*>(&v1);
			m_MaxVertex = const_cast<Vertex*>(&v0);
		}
		else
		{
			throw std::invalid_argument("VertexPair cannot be created with two identical vertices.");
		}
	}

	bool operator==(const VertexPair& other) const
	{
		return (m_MinVertex == other.m_MinVertex && m_MaxVertex == other.m_MaxVertex)
			|| (m_MinVertex == other.m_MaxVertex && m_MaxVertex == other.m_MinVertex);
	}

	~VertexPair() = default;

	/// @brief Get the index of the minimum vertex.
	uint32_t GetMinVertexIndex() const { return m_MinVertex->Index; }

	/// @brief Get the index of the maximum vertex.
	uint32_t GetMaxVertexIndex() const { return m_MaxVertex->Index; }

	/// @brief Get the minimum vertex.
	const Vertex* GetMinVertex() const { return m_MinVertex; }

	/// @brief Get the maximum vertex.
	const Vertex* GetMaxVertex() const { return m_MaxVertex; }

private:
	/// @brief Pointer to the minimum vertex (the one with the smallest index).
	const Vertex* m_MinVertex;
	/// @brief Pointer to the maximum vertex (the one with the largest index).
	const Vertex* m_MaxVertex;
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
		auto h1 = std::hash<const Data::Primitive::Vertex*>{}(vertexPair.GetMinVertex());
		auto h2 = std::hash<const Data::Primitive::Vertex*>{}(vertexPair.GetMaxVertex());
		return h1 ^ h2;
	}
};
} // namespace std
