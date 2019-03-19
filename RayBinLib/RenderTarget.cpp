#include "RenderTarget.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Libs/stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Libs/stb/stb_image_write.h"

#include <stdio.h>

RenderTarget::RenderTarget(int width_, int height_) :
	width(width_),
	height(height_)
{
	buffer = new Vec3[width * height];
}

RenderTarget::~RenderTarget()
{
	if (buffer)
	{
		delete[] buffer;
		buffer = nullptr;
	}
}

void RenderTarget::ApplyGammaCorrection(float gamma)
{
	assert(gamma > 0);
	
	const float invGamma = 1.0f / gamma;
	const int pixelCount = width * height;

	for (int i = 0; i < pixelCount; ++i)
	{
		buffer[i].x = powf(buffer[i].x, invGamma);
		buffer[i].y = powf(buffer[i].y, invGamma);
		buffer[i].z = powf(buffer[i].z, invGamma);
	}
}

void RenderTarget::WritePNG(const char * filename)
{
	printf("Writing RenderTarget to %s...\n", filename);

	int size = width * height;
	const Vec3* pixel = buffer;

	unsigned char* const imgData = new unsigned char[size * 3];

	for (int y = 0; y < height; ++y)
	{
		// img row from top
		unsigned char* p = &imgData[(height - 1 - y) * width * 3];

		for (int x = 0; x < width; ++x, ++pixel)
		{
			//unsigned char* p = &imgData[(height-1-y)*width + x];
			*p++ = int(pixel->x * 255.99f);
			*p++ = int(pixel->y * 255.99f);
			*p++ = int(pixel->z * 255.99f);
		}
	}

	stbi_write_png(filename, width, height, 3, imgData, 0);

	delete imgData;
}
