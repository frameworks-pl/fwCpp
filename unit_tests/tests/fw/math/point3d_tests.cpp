#include "gtest.h"
#include <list>
#include <afxwin.h>
#include <atlstr.h>
#include <src/fw/math/fwpoint3d.h>

TEST(Point3D, subtractTest)
{
	fw::math::Point3D pt1(10.0, 100.0, 200.0);
	fw::math::Point3D pt2(3.4, 55.5, 100.0);

	fw::math::Point3D result = pt1 - pt2;

	ASSERT_EQ(6.6, result.getX());
	ASSERT_EQ(44.5, result.getY());
	ASSERT_EQ(100.0, result.getZ());

}