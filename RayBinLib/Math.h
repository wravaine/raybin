#pragma once

#include <math.h>
#include <assert.h>

#define kPI 3.1415926f

struct Vec3
{
	Vec3() : x(0), y(0), z(0) {}
	Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
	
	float sqLength() const { return x * x + y * y + z * z; }
	float length() const { return sqrtf(x*x + y * y + z * z); }
	void normalize() { float k = 1.0f / length(); x *= k; y *= k; z *= k; }

	Vec3 operator-() const { return Vec3(-x, -y, -z); }
	Vec3& operator+=(const Vec3& o) { x += o.x; y += o.y; z += o.z; return *this; }
	Vec3& operator-=(const Vec3& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
	Vec3& operator*=(const Vec3& o) { x *= o.x; y *= o.y; z *= o.z; return *this; }
	Vec3& operator*=(float o) { x *= o; y *= o; z *= o; return *this; }

	float operator[](int i) const{ return c[i]; }
	float& operator[](int i) { return c[i]; }

	static const Vec3 ZERO;
	static const Vec3 ONE;

	// RIGHT HANDED WORLD COORD SYSTEM (X+ = Right, Y+ = Up, Z- = Forward into screen) 
	static const Vec3 WORLD_ORIGIN;
	static const Vec3 WORLD_UP;
	static const Vec3 WORLD_FORWARD;

	union
	{
		float c[3];
		struct {
			float x, y, z;
		};
	};
	
};

inline Vec3 operator+(const Vec3& a, const Vec3& b) { return Vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
inline Vec3 operator-(const Vec3& a, const Vec3& b) { return Vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
inline Vec3 operator*(const Vec3& a, const Vec3& b) { return Vec3(a.x*b.x, a.y*b.y, a.z*b.z); }
inline Vec3 operator*(const Vec3& a, float b) { return Vec3(a.x*b, a.y*b, a.z*b); }
inline Vec3 operator*(float a, const Vec3& b) { return Vec3(a*b.x, a*b.y, a*b.z); }

inline void AssertUnit(const Vec3& v)
{
	assert(fabsf(v.sqLength() - 1.0f) < 0.01f);
}

inline float dot(const Vec3& a, const Vec3& b) 
{ 
	return a.x*b.x + a.y*b.y + a.z*b.z; 
}

inline Vec3 cross(const Vec3& a, const Vec3& b)
{
	return Vec3(
		a.y*b.z - a.z*b.y,
		-(a.x*b.z - a.z*b.x),
		a.x*b.y - a.y*b.x
	);
}

inline Vec3 normalize(const Vec3& v) 
{ 
	float k = 1.0f / v.length(); 
	return Vec3(v.x*k, v.y*k, v.z*k); 
}

inline Vec3 reflect(const Vec3& v, const Vec3& n)
{
	return v - 2 * dot(v, n)*n;
}

inline bool refract(const Vec3& v, const Vec3& n, float nint, Vec3& outRefracted)
{
	AssertUnit(v);
	float dt = dot(v, n);
	float discr = 1.0f - nint * nint * (1 - dt * dt);
	if (discr > 0)
	{
		outRefracted = nint * (v - n * dt) - n * sqrtf(discr);
		return true;
	}
	return false;
}

inline float schlick(float cosine, float ri)
{
	float r0 = (1 - ri) / (1 + ri);
	r0 = r0 * r0;
	return r0 + (1 - r0) * powf(1 - cosine, 5);
}

float RandomFloat01();
Vec3 RandomInUnitDisk();
Vec3 RandomInUnitSphere();