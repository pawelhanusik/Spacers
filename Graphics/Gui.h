#pragma once

#include "../Math.h"
#include "Screen.h"

class Gui : public Renderable
{
	vec2 m_pos;
	Screen *m_screen;
	int m_toProcessID;
public:
	std::string m_look;
	Gui(std::string, Screen *screen, vec2 pos = vec2(0, 1));
	~Gui();

	//void move(vec2 a);

	void draw(BasicScreen *screen) override;
	void update() override;
};