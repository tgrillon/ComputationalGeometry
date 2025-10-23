#pragma once

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

#include <bitset>
#include <cstdint>

namespace Core::BaseType
{
using EdgeIndex = uint8_t;
using VertexLocalIndex = uint8_t;
using VertexIndex = uint32_t;
using TriangleIndex = uint32_t;
using ScalarValue = double;

using iVec2 = glm::ivec2;

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;

using Mat4 = glm::mat4;
} // namespace Core::BaseType

namespace std
{
/// Hash function for Vec2 to be used in unordered containers.
template<>
struct hash<Core::BaseType::Vec2>
{
	size_t operator()(const Core::BaseType::Vec2& elt) const
	{
		auto h1 = std::hash<Core::BaseType::VertexIndex>{}(elt.x);
		auto h2 = std::hash<Core::BaseType::VertexIndex>{}(elt.y);
		return h1 ^ h2;
	}
};

/// Hash function for Vec3 to be used in unordered containers.
template<>
struct hash<Core::BaseType::Vec3>
{
	size_t operator()(const Core::BaseType::Vec3& elt) const
	{
		auto h1 = std::hash<Core::BaseType::VertexIndex>{}(elt.x);
		auto h2 = std::hash<Core::BaseType::VertexIndex>{}(elt.y);
		auto h3 = std::hash<Core::BaseType::VertexIndex>{}(elt.z);
		return h1 ^ h2 ^ h3;
	}
};
} // namespace std
