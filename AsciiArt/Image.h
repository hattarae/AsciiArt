#pragma once
#include <iostream>
#pragma warning(disable: 4996) //disable deprication warning

//TODO: change from struct to class

enum ImageType
{
	PNG, JPG, BMP, TGA,
};

struct Image
{
	uint8_t* data = NULL;
	size_t size = 0;
	int width;
	int height;
	int channels;

	Image(const char* filename);
	Image(int width, int height, int channels);
	Image(const Image& img);
	~Image();

	bool read(const char* filename);
	bool write(const char* filename);

	ImageType getFileType(const char* filename);

	Image& grayscaleAvg();
	Image& grayscaleLum();
	int getGray(int x, int y, int ver);
	Image& resize(float widthScale, float heightScale);

	void freeImage();
};