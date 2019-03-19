#pragma once

#include "Math.h"

class RenderTarget
{
public:

	RenderTarget(int width_, int height_);
	virtual ~RenderTarget();

	void ApplyGammaCorrection(float gamma);
	void WritePNG(const char* filename);

	int width = 0;
	int height = 0;
	Vec3* buffer = nullptr;
};

