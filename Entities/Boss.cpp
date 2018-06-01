#include "Boss.h"

Boss::Boss(Screen *screen, Physics *physics, vec2 pos, const vec2 *destPos)
	:Entity(
		"       <=====]\n  __ _,-----<\n <..~. =()()=]\n      '-----<\n       <=====]\n"
	, screen, pos),
	m_physics(physics),
	m_destPos(destPos)
{
	m_collisionID = m_physics->collision.addObjectToCollide(this);
}
Boss::~Boss()
{
	m_physics->collision.removeObjectFromCollision(m_collisionID);
}

void Boss::update()
{
	static int cooldown = 3;
	if (--cooldown <= 0) {
		//shoot
		Bullet *bullet = new Bullet(false, m_screen, m_physics, vec2(m_pos.x, m_pos.y+2));
		cooldown = 5;
	} else
	{
		//try not to collide with bullets
		vec2 closestBullet(999, 999);
		auto map = m_physics->collision.getAllObjectsToColide();
		for (std::map<int, Collidable*>::const_iterator it = map.begin(); 
			it != map.end();
			it++)
		{
			Collidable *c = (*it).second;
			if (c == nullptr) continue;
			if (c->getObjectCollisionType() == BULLET) {
				vec2 odl(m_pos.x - c->getPos().x, m_pos.y+2 - c->getPos().y);
				if (odl.x*odl.x + odl.y*odl.y < closestBullet.x*closestBullet.x + closestBullet.y*closestBullet.y ) {
					closestBullet = odl;
				}
			}
		}
		if(closestBullet.x*closestBullet.x + closestBullet.y*closestBullet.y < 20){
			this->move(vec2(0, closestBullet.y < 0 ? -1 : 1));
		}else if(m_pos.y + 2 != m_destPos->y) {
			this->move(vec2(0, m_pos.y+2 - m_destPos->y < 0 ? 1 : -1));
		}

		//special attack
		if (m_hp < 50 && !m_wasSpecialAttackExecuted)
		{
			for(int i = 0; i < 25; ++i)
				Enemy *e = new Enemy(m_screen, m_physics, vec2(m_pos.x + rand() % 50, m_pos.y + rand() % 20 - 10), m_destPos);
			m_hp += 30;

			m_wasSpecialAttackExecuted = true;
		}

		//cant run out of screen
		if (this->m_pos.y <= 0)this->move(vec2(0, 1));
		else if (this->m_pos.y+4 >= m_screen->getHeight())this->move(vec2(0, -1));
		if (this->m_pos.x+14 >= m_screen->getWidth())this->move(vec2(-1, 0));
	}

	//regenerate if havent been hit by bullet for some time
	static int lastHp = m_hp;
	static int regenerationCooldown = 100;
	if (lastHp == m_hp) {
		if (--regenerationCooldown <= 0) {
			if(m_hp < 150) m_hp += 10;
			regenerationCooldown = 100;
		}
	} else {
		regenerationCooldown = 100;
	}

	lastHp = m_hp;
}

const char * Boss::getHitbox() const
{
	return m_look;
}
vec2 Boss::getPos()
{
	return m_pos;
}
void Boss::collide(CollisionObjType type)
{
  	if (type == BULLET) {
		m_hp--;
	}
}

void Boss::explode()
{
	GraphicScreen *gs = m_screen->getGraphicScreen();
	Picture *pic = gs->BMP2Pic("Resources/exBig.bmp");

	int h = 1, w = 0;
	for (w = 0; m_look[w] != 0; ++w) if (m_look[w] == '\n') h++;
	w = w / h;

	pic->m_pos = vec2(m_pos.x*gs->m_width / BasicScreen::m_width, m_pos.y*gs->m_height / BasicScreen::m_height);
	GraphicScreen::img_resize(pic, w * (gs->m_width / m_screen->m_width), h * (gs->m_height / m_screen->m_height));
	m_screen->getGraphicScreen()->splash(pic);
}
