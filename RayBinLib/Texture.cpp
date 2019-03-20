#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Libs/stb/stb_image.h"

Texture::Texture()
{
}

Texture::~Texture()
{
}

//////////////////////////////////////////////////////////////////

FileTexture::FileTexture(const std::string & filename)
{
	this->filename = filename;

	Load(this->filename);
}

FileTexture::~FileTexture()
{
	Unload();
}

bool FileTexture::Load(const std::string & filename)
{
	int channels = 0;
	int w, h;

	// make first pixel the bottom right corner of image
	stbi_set_flip_vertically_on_load(true);	

	if(stbi_uc* fileData = stbi_load(filename.c_str(), &w, &h, &channels, 3))
	{
		Unload();

		this->filename = filename;
		this->width = w;
		this->height = h;
		this->data = new Vec3[w * h];

		int imgSize = w * h * channels;
		Vec3* p = data;
		
		for(int i = 0; i < imgSize; i += channels, ++p)
		{
			p->x = float(fileData[i+0] / 256.0f);
			p->y = float(fileData[i+1] / 256.0f);
			p->z = float(fileData[i+2] / 256.0f);
		}
	}
	
	printf("ERROR: failed to load texture from file: %s\n", filename.c_str());
	return false;
}

Vec3 FileTexture::Sample(float u, float v) const
{
	return Vec3();
}

void FileTexture::Unload()
{
	if(data)
	{
		delete[] data;
		data = nullptr;
	}
}