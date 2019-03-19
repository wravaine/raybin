#pragma once

#include "Math.h"

struct Ray;
struct HitResult;

class Material
{
public:
	virtual bool Scatter(const Ray& ray, const HitResult& hit, Vec3& attenuationOut, Ray& scatteredRayOut) const = 0;
};

namespace Materials
{
	class Lambert : public Material
	{
	public:
		Lambert(Vec3 albedo_) : 
			albedo(albedo_) {}

		virtual bool Scatter(const Ray & ray, const HitResult & hit, Vec3 & attenuationOut, Ray & scatteredRayOut) const override;
		Vec3 albedo;
	};

	class Metalic : public Material
	{
	public:

		Metalic(Vec3 albedo_ = Vec3(1, 1, 1), float roughness_ = 0.f) :
			albedo(albedo_),
			roughness(roughness_) {}

		virtual bool Scatter(const Ray & ray, const HitResult & hit, Vec3 & attenuationOut, Ray & scatteredRayOut) const override;

		Vec3 albedo;
		float roughness = 0.f;
	};
}
