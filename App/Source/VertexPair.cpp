#include "Application/VertexPair.h"

namespace Data::Primitive
{
VertexPair::VertexPair(const BaseType::IndexType v0Idx, const BaseType::IndexType v1Idx)
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

bool VertexPair::operator==(const VertexPair& other) const
{
	return (m_MinVertexIdx == other.m_MinVertexIdx && m_MaxVertexIdx == other.m_MaxVertexIdx)
		|| (m_MinVertexIdx == other.m_MaxVertexIdx && m_MaxVertexIdx == other.m_MinVertexIdx);
}
} // namespace Data::Primitive
