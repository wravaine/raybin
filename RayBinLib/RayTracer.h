#pragma once

#include "Math.h"

#include <vector>

struct Ray;
struct HitResult;
class Material;
class Camera;
class RenderTarget;
struct enkiTaskScheduler;

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

class Raytracer
{
public:

	struct FrameRenderStats
	{
		unsigned long raysFired = 0;
		float elapsedTime = 0;
		float raysPerSecond = 0;
	};

	Raytracer();
	virtual ~Raytracer();

	FrameRenderStats Render(const Camera* camera, RenderTarget* renderTarget);
	
	// pixel coordinate in normalized position
	// du/dv = pixel width/height
	Vec3 ShadePixel(const Camera* camera, float u, float v, float du, float dv, unsigned long& rayCount);
	Vec3 ShadeRay(const Ray& r, int depth, unsigned long& rayCount);
	Vec3 GetBackgroundColor(const Ray& r) const;

	bool Hit(const Ray& r, HitResult& hitResult, float tMin = 0.f, float tMax = FLT_MAX) const;
	bool HitSphere(const Sphere& s, const Ray& r, HitResult& hitResult, float tMin = 0.f, float tMax = FLT_MAX) const;

	int maxDepth = 50;
	int raysPerPixel = 8;
	std::vector<Sphere> spheres; //TODO: move out scene definition

private:
	static void ShadeRowsJob(uint32_t start, uint32_t end, uint32_t threadnum, void* data_);

	enkiTaskScheduler* taskScheduler = nullptr;
};

