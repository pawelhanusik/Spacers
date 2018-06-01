#pragma once

#include <stdio.h>
#include <cstring>
#include <map>
#include <vector>
#include <Windows.h>
#include <conio.h>
#include <functional>

#include "../Math.h"

class BasicScreen
{
public:
	const static int m_width = 80, m_height = 25;
	char m_canvas[m_width * m_height];
	
	//std::vector<std::pair<int, std::pair<int, int>>> colors;
	
	void placeOnScreen(vec2 pos, const char *toPlace);

	int getWidth() {
		return m_width;
	}
	int getHeight() {
		return m_height;
	}
};

class Renderable
{
public:
	virtual void update() = 0;
	virtual void draw(BasicScreen *screen) = 0;

	//Will be called by Screen class' destructor on all objects that are attached to it's toProcess map
	//Objects wich were initialized on heap should override this method and delete itselfs in it
	//virtual void deleteRenderable() {};
	
	//Determines if an object should be deleted when screen's destructor is called.
	virtual bool deleteOnCleanup() { return false; }
};

class GraphicScreen;
class Screen : public BasicScreen
{
	std::map<int, Renderable*> toProcess;
	std::function<void()> m_drawWinFunc;

	GraphicScreen *m_gScreen = nullptr;
public:

	Screen();
	~Screen();

	int addElementToProcess(Renderable *renderable);
	void removeElementToProcess(int idToRemove);

	GraphicScreen* getGraphicScreen();

	void update();
	void draw();
	void clear();
private:
	inline bool IsWindows8OrGreater();
};

//==================================
struct Picture
{
	vec2 m_pos;
	int m_width, m_height;
	COLORREF *pixels;
	Picture(vec2 pos, int width, int height, COLORREF *pixels)
		:m_pos(pos), m_width(width), m_height(height),
		pixels(pixels)
	{
		//pixels = new COLORREF[m_width*m_height];
	}
	Picture()
		:m_pos(vec2(0, 0)), m_width(0), m_height(0),
		pixels(nullptr)
	{}
	~Picture() {
		delete pixels;
	}

	//void setPixel(vec2 pos, COLORREF color);
	//void setPixel(vec2 pos, int r, int g, int b);
};

class GraphicScreen
{
	HWND m_console;
	HDC m_hdc;
	//COLORREF m_bgColour = RGB(0, 0, 0); //unused

	std::vector<Picture*> m_picturesToDraw;

public:
	Screen m_screen;
	///const static int m_width = 1080, m_height = 600;
	int m_width, m_height;

	GraphicScreen();
	~GraphicScreen();

	void placeOnScreen(Picture *picture);

	void draw();
	void clear();

	Picture* BMP2Pic(const char* imgPath);
	int splash(const char* imgPath, int x, int y);
	int splash(Picture *&pic);
	static void img_resize(Picture *pic, int newWidth, int newHeight);
	static void img_flip(Picture *pic);
	static void img_flip_horizontally(Picture *pic);
	static void img_flip_vertically(Picture *pic);
	vec2 basicPos2graficPos(vec2 v) {
		return vec2((v.x - 1)*this->m_width / BasicScreen::m_width, (v.y - 1)*this->m_height / BasicScreen::m_height);
	}
	int fastDraw(Picture &pic);

	/*void setBackgroundColour(int r, int g, int b) {
		m_bgColour = RGB(r, g, b);
	}*/
	int getWidth() {
		return m_width;
	}
	int getHeight() {
		return m_height;
	}
};