#pragma once

#include "Entity.h"
#include "../Physics/Physics.h"

class Enemy : public Entity, Collidable
{
	Physics *m_physics;
	int m_collisionID;
	const vec2 *m_destPos;
	bool m_sf;
	Picture m_pic;
public:
	Enemy(Screen *screen, Physics *physics, vec2 pos, const vec2 *destPos = nullptr);
	~Enemy();

	void update() override;

	const char* getHitbox() const override;
	vec2 getPos() override;
	void collide(CollisionObjType type) override;
	CollisionObjType getObjectCollisionType() override { return ENEMY; }
};