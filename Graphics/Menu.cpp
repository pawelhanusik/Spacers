#include "Menu.h"
#include <iostream>
Menu::Menu(void(*onEnterPressed)(int), Screen *screen, Controls *controls)
	:m_screen(screen),
	m_controls(controls),
	m_onEnterPressed(onEnterPressed)
{
	setWidth(m_screen->getWidth());
	setHeight(m_screen->getHeight());
}
Menu::~Menu()
{
	makeHidden();
}


void Menu::addElement(std::string ele)
{
	m_elements.push_back(ele);
}

void Menu::makeVisible()
{
	m_toProcessID = m_screen->addElementToProcess(this);
	m_toControlID = m_controls->addElementToControl(this);
}

void Menu::makeHidden()
{
	m_screen->removeElementToProcess(m_toProcessID);
	m_controls->removeElementToControl(m_toControlID);
}

void Menu::control(char keyPressed)
{
	if (keyPressed == 'w') {
		m_selectedItemID--;
		if (m_selectedItemID < 0) m_selectedItemID = 0;
	} else if (keyPressed == 's') {
		m_selectedItemID++;
		if (m_selectedItemID >= m_elements.size()) m_selectedItemID = m_elements.size()-1;
	} else if (keyPressed == 13) {
		m_onEnterPressed(m_selectedItemID);
	}
}
void Menu::update()
{
}
void Menu::draw(BasicScreen *screen)
{
	int tmpY = (m_height - m_elements.size()) / 2;
	int itemID = 0;
	for (std::vector<std::string>::iterator it = m_elements.begin(); it != m_elements.end(); ++it, ++tmpY, ++itemID)
	{
		int ods = (m_width - (*it).length()) / 2;
		if (itemID != m_selectedItemID) {
			screen->placeOnScreen(vec2(m_pos.x + ods, m_pos.y + tmpY), (*it).c_str());
		} else {
			std::string toPlace = "> " + (*it) + " <";
			screen->placeOnScreen(vec2(m_pos.x + ods - 2, m_pos.y + tmpY), toPlace.c_str());
		}
	}
}
