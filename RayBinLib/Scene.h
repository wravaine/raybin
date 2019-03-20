#pragma once

#include "Math.h"
#include "Sphere.h"

#include <vector>

struct Ray;
struct HitResult;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	void Add(Sphere s);
	bool Hit(const Ray& r, HitResult& hitResult, float tMin, float tMax) const;

private:
	bool HitSphere(const Sphere& s, const Ray& r, HitResult& hitResult, float tMin, float tMax) const;

	std::vector<Sphere> spheres;
};

