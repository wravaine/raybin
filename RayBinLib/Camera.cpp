#include "Camera.h"

// vfov = vertical field of view angle in degrees (top to bottom)
// aspect = width/height

void Camera::Init(Vec3 lookFrom, Vec3 lookAt, float vfov, float aspect, float aperture, float focalDistance, Vec3 approxUp)
{
	this->origin = lookFrom;
	this->lensRadius = 0.5f * aperture;
	this->focalDistance = focalDistance;

	float theta = vfov * float(kPI / 180); // deg2rad
	this->halfHeight = tanf(0.5f * theta);
	this->halfWidth = aspect * halfHeight;

	LookAtPoint(lookAt);
}

void Camera::LookAtDir(Vec3 dir, Vec3 approxUp)
{
	forward = normalize(dir);
	Vec3 approxRight = normalize(cross(forward, approxUp));
	up = normalize(cross(approxRight, forward));
	right = cross(forward, up);

	horizontal = 2 * halfWidth * focalDistance * right;
	vertical = 2 * halfHeight * focalDistance * up;
	bottomLeft = origin + focalDistance * forward - 0.5f * (horizontal + vertical);
}

void Camera::LookAtPoint(Vec3 pos, Vec3 approxUp)
{
	LookAtDir(normalize(pos - origin));
}

Ray Camera::GetRay(float u, float v) const
{
	Vec3 randPosOnLens = lensRadius * RandomInUnitDisk();
	Vec3 offset = randPosOnLens.x * right + randPosOnLens.y * up;

	Vec3 p = origin + offset;
	return Ray(p, normalize(bottomLeft + u * horizontal + v * vertical - p));
}
