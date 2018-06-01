#include "Bullet.h"

Bullet::Bullet(bool goRight, Screen *screen, Physics *physics, vec2 pos)
	:Entity("-", screen, pos),
	m_directionRight(goRight),
	m_screen(screen),
	m_physics(physics)
{
	m_collisionID = m_physics->collision.addObjectToCollide(this);
}

Bullet::~Bullet()
{
	m_physics->collision.removeObjectFromCollision(m_collisionID);
}

void Bullet::update()
{
	this->move(vec2(m_directionRight ? 1 : -1, 0 ));
	if (m_pos.x > m_screen->getWidth() || m_pos.x < 0 ||
		m_pos.y > m_screen->getHeight() || m_pos.y < 0
		) {
		delete this;
	}
	
}

const char * Bullet::getHitbox() const
{
	return "a";
	/*size_t len = strlen(m_look);
	char *hitbox = new char[len+1];
	int i = 0;
	while (m_look[i] != 0)
	{
		if (m_look[i] != '\n') hitbox[i] = m_look[i] != 32 ? 'a' : ' ';
		else hitbox[i] = '\n';
		++i;
	}
	hitbox[len] = '\0';
	//hitbox = "a";
	return hitbox;*/
}

vec2 Bullet::getPos()
{
	return m_pos;
}

void Bullet::collide(CollisionObjType type)
{
	delete this;
}
