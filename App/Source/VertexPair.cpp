#include "Application/VertexPair.h"

using namespace Core::BaseType;

namespace Data::Primitive
{
VertexPair::VertexPair(const VertexIndex firstIndex, const VertexIndex secondIndex)
{
	if(firstIndex < secondIndex)
	{
		m_MinVertexIdx = firstIndex;
		m_MaxVertexIdx = secondIndex;
	}
	else if(firstIndex > secondIndex)
	{
		m_MinVertexIdx = secondIndex;
		m_MaxVertexIdx = firstIndex;
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

VertexIndex VertexPair::GetMinVertexIdx() const
{
	return m_MinVertexIdx;
}

VertexIndex VertexPair::GetMaxVertexIdx() const
{
	return m_MaxVertexIdx;
}
} // namespace Data::Primitive
