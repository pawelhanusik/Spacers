/*
Code found at:
https://stackoverflow.com/questions/9296059/read-pixel-value-in-bmp-file/17040962#17040962
helped me a lot here :D
*/
#pragma once

#include <fstream>

Picture* loadBMP_custom(const char* imgPath, vec2 pic_pos = vec2(0, 0) )
{
	FILE* f = fopen(imgPath, "rb");

	if (f == NULL)
		throw "Argument Exception";

	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

											   // extract image height and width from header
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

	int row_padded = (width * 3 + 3) & (~3);
	unsigned char* data = new unsigned char[row_padded];
	unsigned char tmp;

	COLORREF *pixels = new COLORREF[width*height];///M
	for (int i = 0; i < height; i++)
	{
		fread(data, sizeof(unsigned char), row_padded, f);
		for (int j = 0; j < width * 3; j += 3)
		{
			// Convert (B, G, R) to (R, G, B)
			/*tmp = data[j];
			data[j] = data[j + 2];
			data[j + 2] = tmp;*/
			
			pixels[i *width + j/3] = RGB(data[j + 2], data[j + 1], data[j + 0]);///M
		}
	}

	fclose(f);
	//return data;

	delete data, f;
	//COLORREF *pixels = new COLORREF[width*height];
	//for (int i = 0; i < width*height; ++i)
	//{
	//	pixels[i] = RGB(data[3 * i + 0], data[3 * i + 1], data[3 * i + 2]);
	//}

	Picture *picture2ret = new Picture(pic_pos, width, height,
		pixels);
	GraphicScreen::img_flip_vertically(picture2ret);
	return picture2ret;
}