#include "Collision.h"

CollisionMapCell Collision::addToCollisionMap(int x, int y, CollisionMapCell val)
{
	/*addWidth(x - m_width + 1);
	addHeight(y - m_height + 1);
	
	CollisionMapCell ret = (m_colissionMap.at(x).at(y) == 0);
	m_colissionMap.at(x).at(y) = val;

	return ret;*/

	CollisionMapCell ret = m_colissionMap[std::pair<int, int>(x, y)];
	m_colissionMap[std::pair<int, int>(x, y)] = val;
	return ret;
}


Collision::Collision()
{
	//addWidth(130);
	//addHeight(40);
}
Collision::~Collision()
{
}

int Collision::addObjectToCollide(Collidable * collidable)
{
	static int newCollidableID = 0;
	newCollidableID++;
	toCollide[newCollidableID] = collidable;
	return newCollidableID;
}
void Collision::removeObjectFromCollision(int idToRemove)
{
	toCollide.erase(idToRemove);
}

void Collision::physicsUpdate()
{
	m_colissionMap.clear();

	std::map<int, Collidable*> tmpToCollide = toCollide;

	//int collisionObjID = 1;
	for(std::map<int, Collidable*>::const_iterator it = tmpToCollide.begin(); it != tmpToCollide.end(); ++it)
	{
		Collidable* c = (*it).second;
		if (c == nullptr) continue;
		
		int collisionObjID = (*it).first;

		vec2 cPos = c->getPos();
		vec2 tmpPos = cPos;
		const char *toPlace = c->getHitbox();
		///addToCollisionMap(tmpPos.x, tmpPos.y, 1);
		int i = 0;
		while (toPlace[i] != 0) {
			while (toPlace[i] == '\n') {
				++i;
				++tmpPos.y;
				tmpPos.x = cPos.x;
			}
			if (toPlace[i] != 32) {
				CollisionMapCell ret = addToCollisionMap(tmpPos.x, tmpPos.y, collisionObjID);
				if ( ret != 0){
					if (toCollide[collisionObjID] != NULL && toCollide[ret] != nullptr)
					{
						CollisionObjType tmpCOT = c->getObjectCollisionType();
						c->collide(tmpToCollide[ret]->getObjectCollisionType());
						toCollide[ret]->collide(tmpCOT);
					}
				}
			}
			++i;
			++tmpPos.x;
		}
		//delete toPlace;
		

		//collisionObjID++;
	}
}
