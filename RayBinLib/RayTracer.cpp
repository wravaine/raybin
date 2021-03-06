#include "RayTracer.h"

#include "Math.h"
#include "Ray.h"
#include "HitResult.h"
#include "Material.h"
#include "RenderTarget.h"
#include "Camera.h"
#include "Scene.h"

#include "Libs/enkiTS/TaskScheduler_c.h"

#include <vector>
#include <atomic>
#include <chrono>

// shared between all render jobs
struct RenderJobData
{
	Raytracer* rayTracer = nullptr;
	RenderTarget* renderTarget = nullptr;
	const Camera* camera = nullptr;

	// concurently writen to by multiple tasks
	std::atomic<unsigned long> rayCount;
};

Raytracer::Raytracer()
{
	taskScheduler = enkiNewTaskScheduler();
	enkiInitTaskScheduler(taskScheduler);
}

Raytracer::~Raytracer()
{
	enkiDeleteTaskScheduler(taskScheduler);
	taskScheduler = nullptr;
}

Raytracer::FrameRenderStats Raytracer::Render(const Scene* scene, const Camera* camera, RenderTarget* renderTarget)
{
	assert(scene);
	assert(renderTarget);
	assert(camera);

	this->scene = scene;

	using clock = std::chrono::high_resolution_clock;
	auto startTime = clock::now();

	RenderJobData jobData;
	jobData.rayTracer = this;
	jobData.renderTarget = renderTarget;
	jobData.camera = camera;

	enkiTaskSet* task = enkiCreateTaskSet(taskScheduler, ShadeRowsJob);
	enkiAddTaskSetToPipeMinRange(taskScheduler, task, &jobData, renderTarget->height, 4);
	enkiWaitForTaskSet(taskScheduler, task);
	enkiDeleteTaskSet(task);

	// gamma correction - could probably thread this?
	renderTarget->ApplyGammaCorrection(2.0f);

	auto endTime = clock::now();
	
	FrameRenderStats stats;
	stats.raysFired = jobData.rayCount;
	stats.elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() * 0.001f;
	stats.raysPerSecond = stats.raysFired / stats.elapsedTime;

	return stats;
}

void Raytracer::ShadeRowsJob(uint32_t start, uint32_t end, uint32_t threadnum, void* data_)
{
	printf("RenderJob#%d - ShadeRows(%d -> %d)\n", threadnum, start, end);

	RenderJobData* jobData = (RenderJobData*)data_;

	float du = 1.0f / jobData->renderTarget->width;
	float dv = 1.0f / jobData->renderTarget->height;
	Vec3* pixel = jobData->renderTarget->buffer + start * jobData->renderTarget->width;

	unsigned long raysFired = 0;

	for (uint32_t y = start; y < end; ++y)
	{
		float v = y / (float)(jobData->renderTarget->height - 1);

		for (int x = 0; x < jobData->renderTarget->width; ++x)
		{
			float u = x / (float)(jobData->renderTarget->width - 1);

			*pixel++ = jobData->rayTracer->ShadePixel(jobData->camera, u, v, du, dv, raysFired);
		}
	}

	printf("RenderJob#%d - done!\n", threadnum);

	// update thread-shared variable
	jobData->rayCount += raysFired;
}

// pixel coordinate in normalized position
// du/dv = pixel width/height
// note: output is still in linear color space (not gamma corrected yet)
Vec3 Raytracer::ShadePixel(const Camera* camera, float u, float v, float du, float dv, unsigned long & rayCount)
{
	//assert(raysPerPixel > 0 && raysPerPixel < 1000);
	Vec3 color;

	if (raysPerPixel > 1)
	{
		for (int i = 0; i < raysPerPixel; ++i)
		{
			float ru = u + du * RandomFloat01();
			float rv = v + dv * RandomFloat01();
			Ray r = camera->GetRay(ru, rv);
			color += ShadeRay(r, 0, rayCount);
		}

		// average colors
		color *= 1.0f / raysPerPixel;
	}
	else
	{
		Ray r = camera->GetRay(u, v);
		color = ShadeRay(r, 0, rayCount);
	}

	return color;
}

Vec3 Raytracer::ShadeRay(const Ray& r, int depth, unsigned long& rayCount)
{
	++rayCount;

	HitResult hit;
	if (Hit(r, hit, 0.0001f))	// 0.0001f removes shadow acne, so we dont hit the object we are reflecting from
	{
		Vec3 attenuation;
		Ray scatterRay;

		if (depth < maxDepth)
		{
			if (hit.material && hit.material->Scatter(r, hit, attenuation, scatterRay))
			{
				return attenuation * ShadeRay(scatterRay, depth + 1, rayCount);
			}
		}

		return Vec3();
	}

	return GetBackgroundColor(r);
}

Vec3 Raytracer::GetBackgroundColor(const Ray& r) const
{
	const Vec3 bottomColor(0.8f, 0.8f, 1.0f);
	const Vec3 topColor(0.2f, 0.2f, 1.0f);

	float v = abs(dot(r.dir, Vec3::WORLD_UP));
	return v * topColor + (1 - v) * bottomColor;
}

bool Raytracer::Hit(const Ray& r, HitResult& hitResult, float tMin, float tMax) const
{
	return scene->Hit(r, hitResult, tMin, tMax);
}
