#pragma once

#include "Application/BaseType.h"

#include <cmath>
#include <limits>

namespace Math
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
} // namespace Math
