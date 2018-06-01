#pragma once

#include <vector>
#include <string>

#include "../Graphics/Screen.h"
#include "../Controls.h"
#include "../Math.h"

class Menu : public Controllable, Renderable
{
	int m_width, m_height;
	vec2 m_pos;
	int m_selectedItemID = 0;
	std::vector<std::string> m_elements;

	int m_toProcessID;
	int m_toControlID;
	Screen *m_screen;
	Controls *m_controls;

	void(*m_onEnterPressed)(int);
public:
	Menu(void (*onEnterPressed)(int), Screen *screen, Controls *controls);
	~Menu();

	void addElement(std::string ele);
	void setPos(vec2 pos) {
		m_pos = pos;
	}
	void setWidth(int width) {
		m_width = width;
	}
	void setHeight(int height) {
		m_height = height;
	}

	void makeVisible();
	void makeHidden();

	void control(char keyPressed) override;	
	void update() override;
	void draw(BasicScreen *screen) override;
};