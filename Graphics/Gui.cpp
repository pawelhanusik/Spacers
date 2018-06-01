#include "Gui.h"

Gui::Gui(std::string look, Screen *screen, vec2 pos)
	:m_pos(pos),
	m_look(look),
	m_screen(screen)
{
	m_toProcessID = m_screen->addElementToProcess(this);
}
Gui::~Gui()
{
	m_screen->removeElementToProcess(m_toProcessID);
}

void Gui::draw(BasicScreen *screen)
{
	screen->placeOnScreen(m_pos, m_look.c_str());
}

void Gui::update()
{
}
