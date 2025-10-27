#pragma once

#include "Core/BaseType.h"

#include <cmath>
#include <limits>

namespace Core::Math::Compare
{
/// @brief Helper function to compare two floating point variables.
constexpr bool EqualNear(float lhs, float rhs, float eps = 1e-6)
{
	float diff = std::fabs(lhs - rhs);
	if(diff <= eps)
		return true;

	return diff <= (std::max(std::fabs(lhs), std::fabs(rhs)) * eps);
}

/// @brief Helper function to compare two Vec2 variables .
constexpr bool EqualNear(Core::BaseType::Vec2 lhs, Core::BaseType::Vec2 rhs, float eps = 1e-6)
{
	return EqualNear(lhs.x, rhs.x, eps) && EqualNear(lhs.y, rhs.y, eps);
}

/// @brief Helper function to compare two Vec3 variables.
constexpr bool EqualNear(Core::BaseType::Vec3 lhs, Core::BaseType::Vec3 rhs, float eps = 1e-6)
{
	return EqualNear(lhs.x, rhs.x, eps) && EqualNear(lhs.y, rhs.y, eps) && EqualNear(lhs.z, rhs.z, eps);
}
} // namespace Core::Math::Compare

namespace Core::Math::Geometry
{
/// @brief Returns the normalized version of v.
inline Core::BaseType::Vec3 Normalize(const Core::BaseType::Vec3& v)
{
	return glm::normalize(v);
}

/// @brief Returns the length of v.
inline Core::BaseType::ScalarValue Length(const Core::BaseType::Vec3& v)
{
	return glm::length(v);
}

/// @brief Cross product of u and v.
inline Core::BaseType::Vec3 Cross(const Core::BaseType::Vec3& u, const Core::BaseType::Vec3& v)
{
	return glm::cross(u, v);
}

/// @brief Dot product of u and v.
inline Core::BaseType::ScalarValue Dot(const Core::BaseType::Vec3& u, const Core::BaseType::Vec3& v)
{
	return glm::dot(u, v);
}

/// @brief Angle (in randians) between u and v.
inline Core::BaseType::ScalarValue Angle(const Core::BaseType::Vec3& u, const Core::BaseType::Vec3& v)
{
	return glm::angle(u, v);
}
} // namespace Core::Math::Geometry
