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

class FaceNormalExtraData : public SingleDataExtraData<BaseType::Vec3>
{
public:
	std::string GetName() override { return "FaceNormalExtraData"; }
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
