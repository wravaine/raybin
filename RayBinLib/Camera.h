#pragma once

#include "Math.h"
#include "Ray.h"

class Camera
{
public:

	// vfov = vertical field of view angle in degrees (top to bottom)
	// aspect = width/height
	void Init(Vec3 lookFrom,
		Vec3 lookAt,
		float vfov,
		float aspect,
		float aperture,			// half lens radius 
		float focalDistance,
		Vec3 approxUp = Vec3::WORLD_UP);

	void LookAtDir(Vec3 dir, Vec3 approxUp = Vec3::WORLD_UP);
	void LookAtPoint(Vec3 pos, Vec3 approxUp = Vec3::WORLD_UP);

	Ray GetRay(float u, float v) const;

private:
	Vec3 origin;		// eye origin in world space

	Vec3 forward;
	Vec3 right;
	Vec3 up;

	Vec3 bottomLeft;	// bottom left position of screen/projection plane in world space
	Vec3 horizontal;
	Vec3 vertical;

	float halfWidth = 0;
	float halfHeight = 0;

	float lensRadius = 1.0f;
	float focalDistance = 1.0f;
};
