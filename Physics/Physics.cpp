#include "Physics.h"

Physics::Physics()
{
	//collision = new Collision();
}
Physics::~Physics()
{
}

void Physics::update()
{
	collision.physicsUpdate();
}
