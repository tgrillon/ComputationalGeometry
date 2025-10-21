#include "Application/MathHelpers.h"

#include <gtest/gtest.h>

using namespace Math::Compare;
using namespace BaseType;

TEST(MathUtilsTest, EqualNear_ShouldHaveTheExpectedResults)
{
	{ // Float comparisons
		float a = 4.25;
		EXPECT_TRUE(EqualNear(a, 4.25));
		EXPECT_FALSE(EqualNear(a, 4.26));
		float b = 0.001;
		EXPECT_TRUE(EqualNear(b, 0.001));
		EXPECT_FALSE(EqualNear(b, 0.002));
		float c = 100025;
		EXPECT_TRUE(EqualNear(c, 100025));
		EXPECT_FALSE(EqualNear(c, 100026));
	}

	{ // Vec2 comparison returning true
		Vec2 a = { 0.5, 0.35 };
		EXPECT_TRUE(EqualNear(a, { 0.5, 0.35 }));
		EXPECT_FALSE(EqualNear(a, { 0.52, 0.37 }));
		Vec2 b = { 0.004, 0.0056 };
		EXPECT_TRUE(EqualNear(b, { 0.004, 0.0056 }));
		EXPECT_FALSE(EqualNear(b, { 0.005, 0.0053 }));
		Vec2 c = { 22450, 660001 };
		EXPECT_TRUE(EqualNear(c, { 22450, 660001 }));
		EXPECT_FALSE(EqualNear(c, { 22451, 660002 }));
	}

	{ // Vec3 comparison returning true
		Vec3 a = { 0.5, 0.35, 0.08 };
		EXPECT_TRUE(EqualNear(a, { 0.5, 0.35, 0.08 }));
		EXPECT_FALSE(EqualNear(a, { 0.55, 0.38, 0.06 }));
		Vec3 b = { 0.004, 0.0056, 0.00012 };
		EXPECT_TRUE(EqualNear(b, { 0.004, 0.0056, 0.00012 }));
		EXPECT_FALSE(EqualNear(b, { 0.003, 0.0058, 0.00014 }));
		Vec3 c = { 22450, 660001, 55142 };
		EXPECT_TRUE(EqualNear(c, { 22450, 660001, 55142 }));
		EXPECT_FALSE(EqualNear(c, { 22451, 660000, 55141 }));
	}
}
