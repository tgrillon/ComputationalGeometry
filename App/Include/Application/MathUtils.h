#pragma once

#include "Application/BaseType.h"

#include <cmath>
#include <limits>

namespace Math
{
constexpr bool EqualNear(float a, float b, float eps = 1e-6)
{
	float diff = std::fabs(a - b);
	if(diff <= eps)
		return true;

	return diff <= (std::max(std::fabs(a), std::fabs(b)) * eps);
}

constexpr bool EqualNear(BaseType::Vec2 a, BaseType::Vec2 b, float eps = 1e-6)
{
	return EqualNear(a.x, b.x, eps) && EqualNear(a.y, b.y, eps);
}

constexpr bool EqualNear(BaseType::Vec3 a, BaseType::Vec3 b, float eps = 1e-6)
{
	return EqualNear(a.x, b.x, eps) && EqualNear(a.y, b.y, eps) && EqualNear(a.z, b.z, eps);
}
} // namespace Math
