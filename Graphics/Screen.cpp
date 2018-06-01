#include "Screen.h"
#include "../LoadBMP.h"

#define SWAP_PIXELS(x, y) COLORREF tmp = x; x = y; y = tmp

void BasicScreen::placeOnScreen(vec2 pos, const char *toPlace)
{
	int i = 0;
	vec2 tmp(0, 0);
	while (toPlace[i] != 0) {
		while (toPlace[i] == '\n') {
			++i;
			++tmp.y;
			tmp.x = 0;
		}

		if (pos.x + tmp.x < this->getWidth()
			&& pos.x + tmp.x >= 0
			&& pos.y + tmp.y < this->getHeight()
			&& pos.y + tmp.y >= 0)
		{
			this->m_canvas[(pos.y + tmp.y)*getWidth() + pos.x + tmp.x] = toPlace[i];
		}
		++i;
		++tmp.x;
	}
}


Screen::Screen()
{
	if (IsWindows8OrGreater()) {
		///FOR Win10
		m_drawWinFunc = [this]() {
			//fwrite(m_canvas, m_width * m_height - 1, 1, stderr);
			//printf("\n", m_canvas);
			fwrite(m_canvas, m_width * m_height, 1, stderr);
		};
	}
	else {
		///FOR Win7
		m_drawWinFunc = [this]() {
			system("CLS");
			fwrite(m_canvas, m_width * m_height, 1, stderr);
		};
	}

	clear();
}

Screen::~Screen()
{
	std::map<int, Renderable*> tmpToProcess = toProcess;
	for (std::map<int, Renderable*>::iterator it = tmpToProcess.begin(); it != tmpToProcess.end(); ++it)
	{
		Renderable *r = (*it).second;
		if(r->deleteOnCleanup())
			delete r;
	}

	if (m_gScreen) delete m_gScreen;
}

int Screen::addElementToProcess(Renderable *renderable)
{
	//toProcess.push_back(renderable);
	//return toProcess.size() - 1;
	static int newToProcessID = -1;
	newToProcessID++;
	toProcess[newToProcessID] = renderable;
	return newToProcessID;
}

void Screen::removeElementToProcess(int idToRemove)
{
	//toProcess.erase(toProcess.begin() + idToRemove);
	toProcess.erase(idToRemove);
}

GraphicScreen* Screen::getGraphicScreen()
{
	if (!m_gScreen) m_gScreen = new GraphicScreen;
	return m_gScreen;
}


void Screen::clear()
{
	memset(m_canvas, ' ', sizeof(m_canvas));
}
void Screen::update()
{
	//std::vector<Renderable*> tmp = toProcess;
 	//for (std::vector<Renderable*>::const_iterator it = tmp.begin(); it != tmp.end(); ++it) {
	//	(*it)->update();
	//}
	std::map<int, Renderable*> tmp = toProcess;
	for (std::map<int, Renderable*>::const_iterator it = tmp.begin(); it != tmp.end(); ++it) {
		(*it).second->update();
	}
}
void Screen::draw()
{
	//for (std::vector<Renderable*>::const_iterator it = toProcess.begin(); it != toProcess.end(); ++it) {
	//	(*it)->draw(this);
	//}
	for (std::map<int, Renderable*>::const_iterator it = toProcess.begin(); it != toProcess.end(); ++it) {
		(*it).second->draw(this);
	}

	///fwrite(m_canvas, m_width * m_height, 1, stderr);
	m_drawWinFunc();
}

bool Screen::IsWindows8OrGreater()
{
	OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0,{ 0 }, 0, 0 };
	DWORDLONG const dwlConditionMask = VerSetConditionMask(
		VerSetConditionMask(
			VerSetConditionMask(
				0, VER_MAJORVERSION, VER_GREATER_EQUAL),
			VER_MINORVERSION, VER_GREATER_EQUAL),
		VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);
	osvi.dwMajorVersion = HIBYTE(_WIN32_WINNT_WIN8);
	osvi.dwMinorVersion = LOBYTE(_WIN32_WINNT_WIN8);
	osvi.wServicePackMajor = 0;

	return VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, dwlConditionMask) != FALSE;
}
//=============================================
GraphicScreen::GraphicScreen()
{
	m_console = GetConsoleWindow();
	m_hdc = GetDC(m_console);
	
	RECT r;
	GetWindowRect(m_console, &r); //stores the console's current dimensions

	m_width = r.right - r.left;
	m_height = r.bottom - r.top;
}
GraphicScreen::~GraphicScreen()
{
	this->clear();
	ReleaseDC(m_console, m_hdc);
}

void GraphicScreen::placeOnScreen(Picture *picture)
{
	m_picturesToDraw.push_back(picture);
}

void GraphicScreen::draw()
{
	for (Picture *picture : m_picturesToDraw)
	{
		vec2 startPos = picture->m_pos;
		vec2 size = vec2(picture->m_width, picture->m_height);
		for (int x = 0; x < size.x; ++x)
		{
			for (int y = 0; y < size.y; ++y)
			{
				SetPixel(m_hdc, startPos.x + x, startPos.y + y, picture->pixels[x + y*picture->m_width]);
			}
		}
	}
	clear();
}

void GraphicScreen::clear()
{
	for (auto it = m_picturesToDraw.begin(); it != m_picturesToDraw.end(); ++it)
		delete (*it);
	m_picturesToDraw.clear();
}

Picture* GraphicScreen::BMP2Pic(const char* imgPath) {
	return loadBMP_custom(imgPath);
}
int GraphicScreen::splash(const char * imgPath, int x, int y)
{
	Picture *pic = loadBMP_custom(imgPath, vec2(x, y));
	this->placeOnScreen(pic);
	this->draw();

	return 0;
}

int GraphicScreen::splash(Picture *&pic)
{
	this->placeOnScreen(pic);
	this->draw();
	return 0;
}

int GraphicScreen::fastDraw(Picture &pic)
{
	vec2 startPos = pic.m_pos;
	vec2 size = vec2(pic.m_width, pic.m_height);
	for (int x = 0; x < size.x; ++x)
	{
		for (int y = 0; y < size.y; ++y)
		{
			SetPixel(m_hdc, startPos.x + x, startPos.y + y, pic.pixels[x + y*pic.m_width]);
		}
	}
	return 0;
}

void GraphicScreen::img_resize(Picture *pic, int newWidth, int newHeight)
{
	COLORREF *newPixels = new COLORREF[newWidth*newHeight];
	float xmul =  float(pic->m_width) / newWidth;
	float ymul = float(pic->m_height) / newHeight;


	for (int y = 0; y < newHeight; ++y) {
		for (int x = 0; x < newWidth; ++x) {
			///printf("%i %i\t%i => %i\n", x, y, newWidth*y +x, int(newWidth*(y*ymul) +(x*xmul)));
			newPixels[newWidth*y + x] = pic->pixels[pic->m_width* int(y*ymul) + int(x*xmul)];
		}
	}
	
	pic->m_width = newWidth;
	pic->m_height = newHeight;
	delete pic->pixels;
	pic->pixels = newPixels;
}
void GraphicScreen::img_flip(Picture *pic)
{
	int wh = pic->m_width * pic->m_height;
	for (int i = 0; i < wh / 2; ++i) {
		SWAP_PIXELS(pic->pixels[i], pic->pixels[wh - i]);
	}
}
void GraphicScreen::img_flip_horizontally(Picture *pic)
{
	int picW = pic->m_width;
	for (int y = 0; y < pic->m_height; ++y) {
		for (int x = 0; x < picW / 2; ++x) {
			SWAP_PIXELS(pic->pixels[picW*y + x], pic->pixels[picW*y + picW - x]);
		}
	}
}
void GraphicScreen::img_flip_vertically(Picture *pic)
{
	int picW = pic->m_width;
	int picH = pic->m_height;
	for (int x = 0; x < picW; ++x) {
		for (int y = 0; y < pic->m_height / 2; ++y) {
			SWAP_PIXELS(pic->pixels[y*picW + x], pic->pixels[(picH - y - 1)*picW + x]);
		}
	}
}


