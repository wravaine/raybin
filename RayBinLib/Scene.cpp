#include "Scene.h"

#include "Ray.h"
#include "HitResult.h"
#include "Material.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::Add(Sphere s)
{
	s.UpdateDerivedData();
	spheres.push_back(s);
}

bool Scene::Hit(const Ray& r, HitResult& hitResult, float tMin, float tMax) const
{
	int hits = 0;
	HitResult closestHit;
	closestHit.t = tMax;

	for(auto it = spheres.begin(); it != spheres.end(); ++it)
	{
		const Sphere& s = *it;

		HitResult hit;
		if(HitSphere(s, r, hit, tMin, closestHit.t))
		{
			if(hit.t < closestHit.t)
			{
				closestHit = hit;
			}

			++hits;
		}
	}

	hitResult = closestHit;
	return hits > 0;
}

bool Scene::HitSphere(const Sphere & s, const Ray & r, HitResult & hitResult, float tMin, float tMax) const
{
	//assert(s.invRadius == 1.0f / s.radius);
	//AssertUnit(r.dir);

	Vec3 oc = r.origin - s.center;
	float b = dot(oc, r.dir);
	float c = dot(oc, oc) - s.radius*s.radius;
	float discr = b * b - c;
	if(discr > 0)
	{
		float discrSqrt = sqrtf(discr);

		float t = (-b - discrSqrt);
		if(t < tMax && t > tMin)
		{
			hitResult.pos = r.GetPointAt(t);
			hitResult.normal = (hitResult.pos - s.center) * s.invRadius;
			hitResult.t = t;
			hitResult.material = s.material;
			return true;
		}
		t = (-b + discrSqrt);
		if(t < tMax && t > tMin)
		{
			hitResult.pos = r.GetPointAt(t);
			hitResult.normal = (hitResult.pos - s.center) * s.invRadius;
			hitResult.t = t;
			hitResult.material = s.material;
			return true;
		}
	}

	return false;
}

