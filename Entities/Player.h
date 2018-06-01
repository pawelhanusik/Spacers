#pragma once

#include "Entity.h"
#include "../Controls.h"
#include "Bullet.h"
#include "../Physics/Physics.h"

class Player : public Entity, Controllable, Collidable
{
	int m_toControlID;
	Controls *m_controls;
	Physics *m_physics;
	int m_collisionID;
public:
	int m_hp;
	int m_weaponCooldown;
	int m_weaponAmmo;

	Player(char *look, Screen *screen, Controls *controls, Physics *physics);
	~Player();

	void control(char keyPressed) override;

	void update() override;

	const char* getHitbox() const override;
	vec2 getPos() override;
	void collide(CollisionObjType type) override;
	CollisionObjType getObjectCollisionType() override { return PLAYER; }
};