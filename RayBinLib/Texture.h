#pragma once

#include "Math.h"
#include <vector>
#include <string>

class Texture
{
public:
	virtual ~Texture() = default;
	virtual Vec3 Sample(float u, float v) const = 0;
};

class ConstantColorTexture : Texture
{
public:
	ConstantColorTexture(Vec3 color = Vec3::ONE) { this->color = color;	}

	// Inherited via Texture
	virtual Vec3 Sample(float u, float v) const override
	{
		return color;
	}

	Vec3 color;
};

// Texture data loaded from file
class ImageTexture : Texture
{
	ImageTexture() = default;
	ImageTexture(const std::string& filename);
	virtual ~ImageTexture();

	bool Load(const std::string& filename);
	bool IsLoaded() const { return data.size() > 0; }

	// Inherited via Texture
	virtual Vec3 Sample(float u, float v) const override;

private:

	void Unload();

	std::vector<Vec3> data;
	int width = 0;
	int height = 0;
	std::string filename;	
};