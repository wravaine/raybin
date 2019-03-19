#pragma once

#include "Math.h"

class Material;

struct HitResult
{
	Vec3 pos;
	Vec3 normal;
	float t = 0;
	const Material* material = nullptr;
};
