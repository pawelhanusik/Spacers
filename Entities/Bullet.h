#pragma once

#include "Entity.h"
#include "../Physics/Physics.h"

class Bullet : public Entity, Collidable
{
	bool m_directionRight;
	Screen *m_screen;
	Physics *m_physics;
	int m_collisionID;
public:
	Bullet(bool goRight, Screen *screen, Physics *physics, vec2 pos);
	~Bullet();

	void update() override;

	const char* getHitbox() const override;
	vec2 getPos() override;
	void collide(CollisionObjType type) override;
	CollisionObjType getObjectCollisionType() override { return m_directionRight ? BULLET : BULLET_BOSS ; }
};