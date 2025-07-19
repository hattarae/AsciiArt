#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize2.h"
#include "Image.h"

Image::Image(const char* filename)
{
	if (read(filename))
	{
		std::cout << "Read " << filename << '\n';
		size = width * height * channels;
	}
	else
	{
		std::cout << "Failed to read " << filename << '\n';
	}
}

Image::Image(int width, int height, int channels) : width(width), height(height), channels(channels)
{
	size = width * height * channels;
	data = new uint8_t[size];
}

Image::Image(const Image& img) : Image(img.width, img.height, img.channels)
{
	memcpy(data, img.data, img.size);
}

Image::~Image()
{
	stbi_image_free(data);
}

bool Image::read(const char* filename) {
	data = stbi_load(filename, &width, &height, &channels, 0);
	if (!data) {
		std::cerr << "Failed to load " << filename << ": " << stbi_failure_reason() << std::endl;
		width = height = channels = 0;  // Reset to safe defaults
	}
	return data != NULL;
}

bool Image::write(const char* filename) {
	ImageType type = getFileType(filename);
	int success;
	switch (type) {
	case PNG:
		success = stbi_write_png(filename, width, height, channels, data, width * channels);
		break;
	case BMP:
		success = stbi_write_bmp(filename, width, height, channels, data);
		break;
	case JPG:
		success = stbi_write_jpg(filename, width, height, channels, data, 100);
		break;
	case TGA:
		success = stbi_write_tga(filename, width, height, channels, data);
		break;
	}
	if (success != 0) {
		printf("\e[32mWrote \e[36m%s\e[0m, %d, %d, %d, %zu\n", filename, width, height, channels, size);
		return true;
	}
	else {
		printf("\e[31;1m Failed to write \e[36m%s\e[0m, %d, %d, %d, %zu\n", filename, width, height, channels, size);
		return false;
	}
}

ImageType Image::getFileType(const char* filename)
{
	const char* ext = strrchr(filename, '.');
	if (ext != nullptr)
	{
		if (strcmp(ext, ".png") == 0)
		{
			return PNG;
		}
		else if (strcmp(ext, ".jpg") == 0)
		{
			return JPG;
		}
		else if (strcmp(ext, ".bmp") == 0)
		{
			return BMP;
		}
		else if (strcmp(ext, ".tga") == 0)
		{
			return TGA;
		}

	}
		return PNG;
}

Image& Image::grayscaleAvg()
{
	// r+b+g/3

	if (channels < 3)
	{
		std::cout << "Image " << this << " is already grayscale.";
	}
	else
	{
		for (int i = 0; i < size; i += channels)
		{
			int gray = (data[i] + data[i + 1] + data[i + 2]) / 3;
			memset(data+i,gray, 3);
		}
	}
	return *this;
}

Image& Image::grayscaleLum()
{
	if (channels < 3)
	{
		std::cout << "Image has less than 3 channels so, " << this << " is already grayscale.";
	}
	else {
		for (int i = 0; i < size; i += channels) {
			int gray = 0.2126 * data[i] + 0.7152 * data[i + 1] + 0.0722 * data[i + 2];
			memset(data + i, gray, 3);
		}
	}
	return *this;
}

int Image::getGray(int x, int y, int ver)
{
	if (channels < 3)
	{
		return data[y * width + x];
	}
	int pixel = (y * width + x) * channels;
	if (ver == 1) //average grayscale
	{
		return (data[pixel] + data[pixel + 1] + data[pixel + 2]) / 3;
	}
	else if (ver == 2)//lum grayscale 
	{
		return  0.2126 * data[pixel] + 0.7152 * data[pixel + 1] + 0.0722 * data[pixel + 2];
	}
	else
	{
		return -1;//error
	}
		
}

Image& Image::resize(float widthScale, float heightScale) 
{
	int newWidth = static_cast<int>((this->width) * widthScale);
	int newHeight = static_cast<int>((this->height) * heightScale);

	if (newWidth <= 0 || newHeight <= 0)
	{
		std::cout << "Resulting image dimensions would be zero";
		exit(1);
	}

	stbir_pixel_layout channelType;

	switch (channels)
	{
	case 1: 
		channelType = STBIR_1CHANNEL;
		break;
	case 3:
		channelType = STBIR_RGB;
		break;
	case 4:
		channelType = STBIR_RGBA;
		break;
	default:
		std::cout << "Unsupported number of channels";
		exit(1);
	}

	uint8_t* resizedPixels = (uint8_t*)malloc(newWidth*newHeight*channels);
	if (!resizedPixels)
	{
		std::cout << "Bad allocation";
		exit(1);
	}

	stbir_resize_uint8_srgb(data, this->width, this->height, 0, resizedPixels, newWidth, newHeight, 0, channelType);

	freeImage();
	data = resizedPixels;

	this->width = newWidth;
	this->height = newHeight;

	size = newWidth * newHeight * channels;

	return *this;
}

void Image::freeImage()
{
	if (data)
	{
		free(data);
		data = nullptr;
	}
}

int Image::getWidth()
{
	return width;
}

int Image::getHeight()
{
	return height;
}
