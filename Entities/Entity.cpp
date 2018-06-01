#include "Entity.h"
Entity::Entity(char *look, Screen *screen, vec2 pos)
	:m_look(look),
	m_screen(screen),
	m_pos(pos)
{
	m_toProcessID = m_screen->addElementToProcess(this);
}
Entity::~Entity()
{
	m_screen->removeElementToProcess(m_toProcessID);
}

void Entity::move(vec2 a)
{
	m_pos.x += a.x;
	m_pos.y += a.y;
}
void Entity::teleportTo(vec2 pos)
{
	m_pos.x = pos.x;
	m_pos.y = pos.y;
}

void Entity::explode()
{
	GraphicScreen *gs = m_screen->getGraphicScreen();
	Picture *pic = gs->BMP2Pic("Resources/exBig.bmp");
	
	int h=1, w=0;
	for (w = 0; m_look[w] != 0; ++w) if (m_look[w] == '\n') h++;
	w = w / h;

	pic->m_pos = vec2(m_pos.x*gs->m_width / BasicScreen::m_width, m_pos.y*gs->m_height / BasicScreen::m_height);
	GraphicScreen::img_resize(pic, w * (gs->m_width / m_screen->m_width), h * (gs->m_height / m_screen->m_height));
	//m_screen->getGraphicScreen()->splash(pic);
	m_screen->getGraphicScreen()->fastDraw(*pic);
	delete pic;
}


void Entity::draw(BasicScreen *screen)
{
	screen->placeOnScreen(m_pos, m_look);
}
void Entity::update()
{
}

