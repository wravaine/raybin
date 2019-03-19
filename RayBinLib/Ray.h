#pragma once

#include "Math.h"

struct Ray
{
	Ray() {}
	Ray(const Vec3& orig_, const Vec3& dir_) : origin(orig_), dir(dir_) { /* AssertUnit(dir); */ }

	Vec3 GetPointAt(float t) const { return origin + dir * t; }

	Vec3 origin;
	Vec3 dir;
};

