#include "Player.h"

Player::Player(char * look, Screen *screen, Controls *controls, Physics *physics)
	: Entity( look, screen),
	m_controls(controls),
	m_physics(physics)
{
	m_hp = 100;
	m_weaponCooldown = 0;
	m_weaponAmmo = 5;
	m_toControlID = m_controls->addElementToControl(this);
	m_collisionID = m_physics->collision.addObjectToCollide(this);
}

Player::~Player()
{
	m_controls->removeElementToControl(m_toControlID);
	m_physics->collision.removeObjectFromCollision(m_collisionID);
}

void Player::control(char keyPressed)
{
	switch (keyPressed) {
	case 'w':
		//this->m_look = " O\n/|\\\n |\n/ \\";
		this->move(vec2(0, -1));
		break;
	case 's':
		//this->m_look = " O\n\\|/\n |\n/ \\";
		this->move(vec2(0, 1));
		break;
	case 'a':
		//this->m_look = " O\n-|-\n |\n| \\";
		this->move(vec2(-1, 0));
		break;
	case 'd':
		//this->m_look = " O\n-|-\n |\n/ |";
		this->move(vec2(1, 0));
		break;
	case 'p':
		for(int i = 1; i < 30; ++i)
			Bullet *bullet = new Bullet(true, m_screen, m_physics, vec2(m_pos.x + 4, i));
		break;
	
	case 'r':
		m_weaponCooldown = 10;
		break;
	case 32:
		if (m_weaponCooldown == 0 && m_weaponAmmo > 0) {
			Bullet *bullet = new Bullet(true, m_screen, m_physics, vec2(m_pos.x + 4, m_pos.y + 1));
			m_weaponAmmo--;
		}
		break;
	
	case 'f':
		m_hp -= 25;
		break;
	//default:
		//this->m_look = " O\n-|-\n |\n/ \\";
		//break;	
	}

	if (this->m_pos.y <= 0)this->move(vec2(0, 1));
	else if (this->m_pos.y + 4 >= m_screen->getHeight())this->move(vec2(0, -1));
	if (this->m_pos.x < 0)this->move(vec2(1, 0));
	else if (this->m_pos.x >= m_screen->getWidth())this->move(vec2(-1, 0));
}

void Player::update()
{
	if (m_weaponCooldown > 0) {
		m_weaponCooldown--;
		if (m_weaponCooldown == 0) {
			m_weaponAmmo = 10;
		}
	}
	
}

const char * Player::getHitbox() const
{
	
	/*size_t len = strlen(m_look);
	//char *hitbox = new char[len+1];
	char *hitbox = (char*)alloca(len + 1);
	int i = 0;
	while (m_look[i] != 0)
	{
		if (m_look[i] != '\n') hitbox[i] = m_look[i] != 32 ? 'a' : ' ';
		else hitbox[i] = '\n';
		++i;
	}
	hitbox[len] = '\0';*/
	return m_look;
}
vec2 Player::getPos()
{
	return m_pos;
}
void Player::collide(CollisionObjType type)
{
	if (type == ENEMY || type == BULLET_BOSS ) {
		m_hp -= 10;
		if (m_hp <= 0)
			//delete this;
			m_pos = vec2(-200, 15);
	}
}