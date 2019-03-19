#include "Material.h"

#include "Ray.h"
#include "HitResult.h"

namespace Materials
{
	bool Lambert::Scatter(const Ray& ray, const HitResult& hit, Vec3& attenuationOut, Ray& scatteredRayOut) const
	{
		// perfectly matte/diffuse materials reflect incoming light in a random direction
		Vec3 target = hit.pos + hit.normal + RandomInUnitSphere();
		scatteredRayOut = Ray(hit.pos, normalize(target - hit.pos));
		attenuationOut = albedo;
		return true;
	}

	bool Metalic::Scatter(const Ray & ray, const HitResult & hit, Vec3 & attenuationOut, Ray & scatteredRayOut) const
	{
		// perfectly matte/diffuse materials reflect incoming light in a random direction
		Vec3 reflected = reflect(ray.dir, hit.normal);

		if (roughness > 0)
		{
			Vec3 target = hit.pos + reflected + roughness * RandomInUnitSphere();
			scatteredRayOut = Ray(hit.pos, normalize(target - hit.pos));
		}
		else
		{
			scatteredRayOut = Ray(hit.pos, reflected);
		}

		attenuationOut = albedo;
		return dot(scatteredRayOut.dir, hit.normal) > 0; // only scatter rays to bounce off
	}
}
