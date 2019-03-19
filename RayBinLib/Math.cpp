
#include "Math.h"

const Vec3 Vec3::ZERO(0, 0, 0);
const Vec3 Vec3::ONE(1, 1, 1);
const Vec3 Vec3::WORLD_ORIGIN(0, 0, 0);
const Vec3 Vec3::WORLD_UP(0, 1, 0);
const Vec3 Vec3::WORLD_FORWARD(0, 0, 1);

static unsigned int XorShift32()
{
	// important: this can be accessed on different threads at same time 
	// should be unique to each thread so it doesn't cause cache sharing issues
	thread_local static unsigned int s_RndState = 1;

	unsigned int x = s_RndState;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 15;
	s_RndState = x;
	return x;
}

float RandomFloat01()
{
	return (XorShift32() & 0xFFFFFF) / 16777216.0f;
}

Vec3 RandomInUnitDisk()
{
	Vec3 p;
	do
	{
		p = 2.0 * Vec3(RandomFloat01(), RandomFloat01(), 0) - Vec3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}

Vec3 RandomInUnitSphere()
{
	Vec3 p;
	do {
		p = 2.0*Vec3(RandomFloat01(), RandomFloat01(), RandomFloat01()) - Vec3(1, 1, 1);
	} while (p.sqLength() >= 1.0);
	return p;
}

