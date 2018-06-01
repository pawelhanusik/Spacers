#pragma once

#include <map>
#include <unordered_map>

#include "../Math.h"

enum CollisionObjType {PLAYER, ENEMY, BULLET, BULLET_BOSS, BARRIER};

class Collidable {
public:
	virtual const char* getHitbox() const = 0;
	virtual vec2 getPos() = 0;
	virtual void collide(CollisionObjType) = 0;
	virtual CollisionObjType getObjectCollisionType() = 0;
};

typedef unsigned short CollisionMapCell;
class Collision
{
	std::map<int, Collidable*> toCollide;
	
	struct pair_hash {
		template <class T1, class T2>
		std::size_t operator () (const std::pair<T1, T2> &p) const {
			auto h1 = std::hash<T1>{}(p.first);
			auto h2 = std::hash<T2>{}(p.second);

			// Mainly for demonstration purposes, i.e. works but is overly simple
			// In the real world, use sth. like boost.hash_combine
			return h1 ^ h2;
		}
	};
	int m_width = 0, m_height = 0;
	//std::vector<std::vector<CollisionMapCell>> m_colissionMap;
	std::unordered_map<std::pair<int, int>, CollisionMapCell, pair_hash> m_colissionMap;
	
	CollisionMapCell addToCollisionMap(int x, int y, CollisionMapCell val);
public:
	Collision();
	~Collision();

	int addObjectToCollide(Collidable *collidable);
	void removeObjectFromCollision(int idToRemove);
	std::map<int, Collidable*> getAllObjectsToColide() { return toCollide; }

	/*void addWidth(int howMuch = 1);
	void addHeight(int howMuch = 1);*/

	void physicsUpdate();
};