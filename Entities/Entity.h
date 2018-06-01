#pragma once

#include "../Math.h"
#include "../Graphics/Screen.h"


class Entity : public Renderable{
public:
	int m_toProcessID;
	Screen *m_screen;

	vec2 m_pos;
	char *m_look;

	Entity(char *look, Screen *screen, vec2 pos = vec2());
	~Entity();

	virtual void move(vec2 a);
	virtual void teleportTo(vec2 pos);

	virtual void explode();

	void draw(BasicScreen *screen) override;
	void update() override;
	bool deleteOnCleanup() override { return true; };
};