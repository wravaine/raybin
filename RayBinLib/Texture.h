#pragma once

#include "Math.h"
#include <string>

class Texture
{
public:
	Texture();
	virtual ~Texture();

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
class FileTexture : Texture
{
	FileTexture() {}
	FileTexture(const std::string& filename);
	virtual ~FileTexture();

	bool Load(const std::string& filename);
	bool IsLoaded() const { return data != nullptr; }

	// Inherited via Texture
	virtual Vec3 Sample(float u, float v) const override;

private:

	void Unload();

	std::string filename;
	Vec3* data = nullptr;
	int width = 0;
	int height = 0;
};