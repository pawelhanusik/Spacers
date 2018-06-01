#pragma once

#include "Entity.h"
#include "../Physics/Physics.h"
#include "Bullet.h"
#include "Enemy.h"

class Boss : public Entity, Collidable
{
	Physics *m_physics;
	int m_collisionID;
	
	const vec2 *m_destPos;
	bool m_wasSpecialAttackExecuted = false;
public:
	int m_hp = 100;

	Boss(Screen *screen, Physics *physics, vec2 pos, const vec2 *destPos);
	~Boss();

	void update() override;

	const char* getHitbox() const override;
	vec2 getPos() override;
	void collide(CollisionObjType type) override;
	CollisionObjType getObjectCollisionType() override { return ENEMY; }

	void explode();
};