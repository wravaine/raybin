#pragma once

#include "Math.h"

class Material;

struct Sphere
{
	Sphere(Vec3 center_, float radius_, const Material* material_ = nullptr) :
		center(center_),
		radius(radius_),
		invRadius(0.0f),
		material(material_)
	{}

	void UpdateDerivedData() { invRadius = 1.0f / radius; }

	Vec3 center;
	float radius = 1;
	float invRadius = 0;
	const Material* material = nullptr;
};

