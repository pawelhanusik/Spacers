#pragma once

#include <time.h>
#include <math.h>

namespace Utils {
	void gotoxy(int x, int y)
	{
		static HANDLE h = NULL;
		if (!h)
			h = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD c = { x, y };
		SetConsoleCursorPosition(h, c);
	}

	int randint(int mod)
	{
		static bool rand_inited = false;
		if (!rand_inited)srand(time(NULL));
		return rand() % mod;
	}

	/*COLORREF* loadBMP_custom(const char * imagepath) {
		// Data read from the header of the BMP file
		unsigned char header[54]; // Each BMP file begins by a 54-bytes header
		unsigned int dataPos;     // Position in the file where the actual data begins
		unsigned int width, height;
		unsigned int imageSize;   // = width*height*3
								  // Actual RGB data
		unsigned char * data;

		// Open the file
		FILE * file = fopen(imagepath, "rb");
		if (!file) { printf("Image could not be opened\n"); return 0; }

		if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
			printf("Not a correct BMP file\n");
			return false;
		}

		if (header[0] != 'B' || header[1] != 'M') {
			printf("Not a correct BMP file\n");
			return 0;
		}

		// Read ints from the byte array
		dataPos = *(int*)&(header[0x0A]);
		imageSize = *(int*)&(header[0x22]);
		width = *(int*)&(header[0x12]);
		height = *(int*)&(header[0x16]);

		// Some BMP files are misformatted, guess missing information
		if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
		if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

											 // Create a buffer
		data = new unsigned char[imageSize];

		// Read the actual data from the file into the buffer
		fread(data, 1, imageSize, file);

		//Everything is in memory now, the file can be closed
		fclose(file);

		int lineNr = width*height;
		COLORREF *pixels = new COLORREF[width*height];
		for (int i = 0; i + 3 < imageSize; i += 3) {
			pixels[--lineNr] = RGB(data[i + 2], data[i + 1], data[i + 3]);
			//std::cout << lineNr++ << ": " << (int)data[i] << "\t" << (int)data[i + 1] << "\t" << (int)data[i + 2] << "\n";
			if (lineNr <= 0) break;
		}

		delete data, file;

		return pixels;
	}*/

}