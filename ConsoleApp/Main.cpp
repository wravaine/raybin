#include "stdafx.h"

#include "RayBinLib/Material.h"
#include "RayBinLib/RenderTarget.h"
#include "RayBinLib/Camera.h"
#include "RayBinLib/RayTracer.h"
#include "RayBinLib/Scene.h"

int main()
{
	Raytracer rt;
	rt.raysPerPixel = 256;
	rt.maxDepth = 4;

	RenderTarget renderTarget(1024, 768);

	Camera camera;
	Vec3 camPos = Vec3(-2, 2, 1);
	Vec3 camLookAt = Vec3(0, 0, 0);
	float aperture = 0.1f;
	float focalDistance = (camLookAt - camPos).length();
	float fov = 60.0f;
	float aspect = (float)renderTarget.width / renderTarget.height;
	camera.Init(camPos, camLookAt, fov, aspect, aperture, focalDistance);

	Materials::Lambert matRedDiffuse(Vec3(1, 0, 0));
	Materials::Lambert matBlueDiffuse(Vec3(0.3f, 0.4f, 1));
	Materials::Metalic matBlueMetalic(Vec3(0.1f, 0.1f, 1), 0.05f);
	Materials::Lambert matGreenDiffuse(Vec3(0, 1, 0));
	Materials::Metalic matRoughMetalic(Vec3(0.5f, 0.5f, 0.5f), 0.25f);

	Scene scene;
	scene.Add(Sphere(Vec3(0, 0.75f, 0), 0.25f, &matBlueMetalic));
	scene.Add(Sphere(Vec3(0, 0, 0), 0.5f, &matRedDiffuse));
	scene.Add(Sphere(Vec3(1.5f, 0, 1), 0.5f, &matRoughMetalic));
	scene.Add(Sphere(Vec3(0, -100.5f, 0), 100.0f, &matGreenDiffuse));
	scene.Add(Sphere(Vec3(1.5f, 0, -5), 0.5f, &matBlueDiffuse));

	printf("Generating image (%dx%d, %d samples per pixel)\n", renderTarget.width, renderTarget.height, rt.raysPerPixel);

	auto stats = rt.Render(&scene, &camera, &renderTarget);

	printf("Done! Elapsed Time: %.3f sec (%d Mrays fired, %.2f Mrays/s)\n", 
		stats.elapsedTime, 
		stats.raysFired / 1000000,
		stats.raysPerSecond / 1000000.f);

	renderTarget.WritePNG("output.png");

	printf("Done. Press any key to continue...\n");
	getchar();
    return 0;
}

