#pragma once


#include "Collision.h"

class Physics
{
public:
	Collision collision;

	Physics();
	~Physics();

	void update();

	virtual void physicsUpdate() {};
};