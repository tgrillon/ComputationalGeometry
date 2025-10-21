#pragma once

#include "Application/BaseType.h"

#include <cmath>
#include <limits>

namespace Math::Compare
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
constexpr bool EqualNear(BaseType::Vec2 lhs, BaseType::Vec2 rhs, float eps = 1e-6)
{
	return EqualNear(lhs.x, rhs.x, eps) && EqualNear(lhs.y, rhs.y, eps);
}

/// @brief Helper function to compare two Vec3 variables.
constexpr bool EqualNear(BaseType::Vec3 lhs, BaseType::Vec3 rhs, float eps = 1e-6)
{
	return EqualNear(lhs.x, rhs.x, eps) && EqualNear(lhs.y, rhs.y, eps) && EqualNear(lhs.z, rhs.z, eps);
}
} // namespace Math::Compare

namespace Math::Geometry
{
/// @brief Returns the normalized version of v.
inline BaseType::Vec3 Normalize(const BaseType::Vec3& v)
{
	return glm::normalize(v);
}

/// @brief Returns the length of v.
inline BaseType::ScalarValue Length(const BaseType::Vec3& v)
{
	return glm::length(v);
}

/// @brief Cross product of u and v.
inline BaseType::Vec3 Cross(const BaseType::Vec3& u, const BaseType::Vec3& v)
{
	return glm::cross(u, v);
}

/// @brief Dot product of u and v.
inline BaseType::ScalarValue Dot(const BaseType::Vec3& u, const BaseType::Vec3& v)
{
	return glm::dot(u, v);
}

/// @brief Angle (in randians) between u and v.
inline BaseType::ScalarValue Angle(const BaseType::Vec3& u, const BaseType::Vec3& v)
{
	return glm::angle(u, v);
}
} // namespace Math::Geometry
