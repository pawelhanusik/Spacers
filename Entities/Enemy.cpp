#include "Enemy.h"
#include <time.h>
#include <math.h>

Enemy::Enemy(Screen *screen, Physics *physics, vec2 pos, const vec2 *destPos)
	:Entity(
		" __\n<__|",
		//"   \n    ",
		screen, pos),
	m_physics(physics),
	m_destPos(destPos)
{
	m_collisionID = m_physics->collision.addObjectToCollide(this);
}
Enemy::~Enemy()
{
	explode();
	m_physics->collision.removeObjectFromCollision(m_collisionID);
}

void Enemy::update()
{
	if (m_sf) {
		m_pic.m_pos = m_screen->getGraphicScreen()->basicPos2graficPos(m_pos);
		m_screen->getGraphicScreen()->fastDraw(m_pic);
	}

	if (m_destPos == nullptr)
	{
		this->move(vec2(-1, 0));
	}
	else
	{
		if (abs(m_pos.x - (*m_destPos).x) > abs(m_pos.y - (*m_destPos).y)*3 ) {
			this->move(vec2(-1, 0));
		} else {
			if (m_pos.y - (*m_destPos).y > 0) {
				this->move(vec2(0, -1));
			}else {
				this->move(vec2(0, 1));
			}
		}
	}	
}

const char* Enemy::getHitbox() const
{
	return "aaaa\n aa";
	/*size_t len = strlen(m_look);
	char *hitbox = new char[len + 1];
	int i = 0;
	while (m_look[i] != 0)
	{
		if (m_look[i] != '\n') hitbox[i] = m_look[i] != 32 ? 'a' : ' ';
		else hitbox[i] = '\n';
		++i;
	}
	hitbox[len] = '\0';
	return hitbox;*/
}
vec2 Enemy::getPos()
{
	return m_pos;
}
void Enemy::collide(CollisionObjType type)
{
	if(type != ENEMY/* && type != BULLET_BOSS*/)
		delete this;
}
