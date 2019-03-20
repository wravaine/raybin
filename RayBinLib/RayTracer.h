#pragma once

#include "Math.h"

#include <vector>

struct Ray;
struct HitResult;
class Material;
class Camera;
class RenderTarget;
struct enkiTaskScheduler;
class Scene;

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

	FrameRenderStats Render(const Scene* scene, const Camera* camera, RenderTarget* renderTarget);
	
	// pixel coordinate in normalized position
	// du/dv = pixel width/height
	Vec3 ShadePixel(const Camera* camera, float u, float v, float du, float dv, unsigned long& rayCount);
	Vec3 ShadeRay(const Ray& r, int depth, unsigned long& rayCount);
	Vec3 GetBackgroundColor(const Ray& r) const;

	bool Hit(const Ray& r, HitResult& hitResult, float tMin = 0.f, float tMax = FLT_MAX) const;
	
	int maxDepth = 50;
	int raysPerPixel = 8;
	
private:
	static void ShadeRowsJob(uint32_t start, uint32_t end, uint32_t threadnum, void* data_);

	const Scene* scene = nullptr;
	enkiTaskScheduler* taskScheduler = nullptr;
};

