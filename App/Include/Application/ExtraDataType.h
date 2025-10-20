#pragma once

#include "Application/BaseType.h"

#include <string>
#include <vector>

namespace Data::ExtraData
{
class BaseExtraDataType
{
public:
	virtual ~BaseExtraDataType() = default;
	virtual std::string GetName() = 0;
};

template<typename T>
class SingleDataExtraData : public BaseExtraDataType
{
public:
	SingleDataExtraData() = default;
	~SingleDataExtraData() = default;

	T const& GetData() const { return m_Data; }

	T& GetData() { return m_Data; }

	void SetData(T const& value) { m_Data = value; }

private:
	T m_Data;
};

/// @brief Extra data type to store a triangle normal.
class TriangleNormalExtraData : public SingleDataExtraData<BaseType::Vec3>
{
public:
	/// @brief Returns the name of the extra data.
	std::string GetName() override { return "TriangleNormalExtraData"; }
};

class VertexNormalExtraData : public SingleDataExtraData<BaseType::Vec3>
{
public:
	std::string GetName() override { return "VertexNormalExtraData"; }
};

class FlatVertexNormalsExtraData : public SingleDataExtraData<std::vector<BaseType::Vec3>>
{
public:
	std::string GetName() override { return "FlatVertexNormalsExtraData"; }
};

} // namespace Data::ExtraData
