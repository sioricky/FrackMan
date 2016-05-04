#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>
using namespace std;

objectbase::objectbase(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* Wolrd,bool visible)
	:GraphObject(imageID, startX, startY,dir,size,depth),m_world(Wolrd), Dead(false)
{
	setVisible(visible);
};
StudentWorld* objectbase::getWorld()
{
	return m_world;
}
void objectbase::setDead()
{
	if (isDead())
		return;
	Dead = true;
	setVisible(false);
}
void objectbase::setHealth(int health)
{
	m_health = health;
}
int objectbase::getHealth()
{
	return m_health;
}
void objectbase::decHealth(int hit)
{
	m_health = m_health - hit;
//	if (m_health <= 0)
//		setDead();
}

bool objectbase::isDead()
{
	if (Dead)
		return true;
	else 
		return false;
}
Dirt::Dirt(int startX, int startY, StudentWorld* Wolrd)
	:objectbase(IID_DIRT,startX,startY,right,0.25,3, Wolrd,true)
{}

void Dirt::doSomething()
{}

void FrackMan::decBarrel()
{
	m_leftBarrel--;
}
FrackMan::FrackMan(int startX, int startY,int oil,  StudentWorld* Wolrd)
	:objectbase(IID_PLAYER, startX, startY, right, 1.0, 0, Wolrd, true),m_squirt(5),m_leftBarrel(oil), m_gold(0),m_sonar(1)
{
	setHealth(10);
}

void FrackMan::decSonar()
{
	m_sonar--;
}

int FrackMan::getSonar()
{
	return m_sonar;
}

void FrackMan::incSonar()
{
	m_sonar++;
}

int FrackMan::getHealthMan()
{
	return getHealth() * 10;
}
int FrackMan::getSquirt()
{
	return m_squirt;
}
void FrackMan::decGold()
{
	m_gold--;
}
void FrackMan::decSquirt()
{
	m_squirt--;
}

void FrackMan::addGold()
{
	m_gold++;
}
int FrackMan::getGold()
{
	return m_gold;
}
void FrackMan::addSquirt()
{
	m_squirt = m_squirt + 5;
}

void FrackMan::doSomething()
{
	if (isDead())
		return;
	int ch;
	int x_f = getX();
	int y_f = getY();

	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_ESCAPE:
			setDead();
			break;
		case KEY_PRESS_LEFT :
			if (getDirection() == left&&x_f - 1 >= 0&& !getWorld()->withinBoulder(x_f - 1, y_f,0))
				moveTo(x_f - 1, y_f);
			else {
				setDirection(left);
				moveTo(x_f, y_f);
			}
				break;
		case KEY_PRESS_RIGHT :
			if (getDirection() == right&&x_f + 1 <= Frack_S&& !getWorld()->withinBoulder(x_f + 1, y_f,0))
				moveTo(x_f + 1, y_f);
			else {
				setDirection(right);
				moveTo(x_f, y_f);
			}
			break;
		case KEY_PRESS_UP :
			if (getDirection() == up&&y_f + 1 <=Frack_S&& !getWorld()->withinBoulder(x_f, y_f+1,0))
				moveTo(x_f, y_f + 1);
			else {
				setDirection(up);
				moveTo(x_f, y_f);
			}
			break;
		case KEY_PRESS_DOWN :
			if (getDirection() == down&&y_f - 1 >= 0 && !getWorld()->withinBoulder(x_f , y_f-1,0))
				moveTo(x_f, y_f - 1);
			else {
				setDirection(down);
				moveTo(x_f, y_f);
			}
			break;
		case KEY_PRESS_SPACE:
			getWorld()->fireSquirt();
			break;
		case KEY_PRESS_TAB:
			getWorld()->giveBribe();
			break;
		case'z':
		case'Z':
			getWorld()->fireSonar();
			decSonar();
			break;
		}
	}
	if (getWorld()->occuppied(getX(), getY()))
	{
		getWorld()->playSound(SOUND_DIG);
		getWorld()->removeDirt(x_f, y_f);
	}
}
Squirt::Squirt(int startX, int startY, Direction dir, StudentWorld* World)
	:objectbase(IID_WATER_SPURT, startX, startY, dir, 1.0, 1, World, true)
{}
void Squirt::doSomething()
{
	m_count++;
	if (m_count > 5)
		setDead();
	if (isDead())
		return;
	StudentWorld* m_world = getWorld();
	Direction d = getDirection();
	int x_s = getX();
	int y_s = getY();
	getWorld()->squirtHit(this);
	if (m_world->occuppied(x_s, y_s)||!m_world->withinView(x_s,y_s)||m_world->withinBoulder(x_s,y_s,0))
		setDead();
	if (m_count > 1)
	{
		if (d == left&&!m_world->occuppied(x_s - 1, y_s)&&m_world->withinView(x_s - 1, y_s)&&!m_world->withinBoulder(x_s, y_s, 0))
			moveTo(x_s - 1, y_s);
		else if (d == right&&!m_world->occuppied(x_s + 1, y_s) && m_world->withinView(x_s + 1, y_s)&&!m_world->withinBoulder(x_s, y_s, 0))
			moveTo(x_s + 1, y_s);
		else if (d == up&&!m_world->occuppied(x_s, y_s + 1) && m_world->withinView(x_s, y_s + 1) && !m_world->withinBoulder(x_s, y_s, 0))
			moveTo(x_s, y_s + 1);
		else if (d == down&&!m_world->occuppied(x_s, y_s - 1) && m_world->withinView(x_s, y_s - 1) && !m_world->withinBoulder(x_s, y_s, 0))
			moveTo(x_s, y_s - 1);
		else
			setDead();
	}
}
Boulder::Boulder(int startX, int startY, StudentWorld* World)
	:objectbase(IID_BOULDER, startX, startY, down, 1.0, 1, World,true),m_state(stable),m_count(0)
{
}
void Boulder::doSomething()
{
	if (isDead())
		return;
	int x = getX();
	int y = getY();
	if (!getWorld()->occuppied(x, y - 1))
		m_state = waiting;
	if (m_count > 30)
	{
		m_state = falling;
	}
	switch (m_state)
	{
	case(stable) :
		break;
	case(waiting) :
		m_count++;
		break;
	case(falling) :
		moveBoulder();
		break;
	}
}
void Boulder::moveBoulder()
{

	int x = getX();
	int y = getY();
	if (m_flag == 1)
	{
		getWorld()->playSound(SOUND_FALLING_ROCK);
		m_flag = 2;
	}
	if (m_flag == 0)
		setDead();
	else if (getWorld()->occuppied(x, y - 1))
	{
		m_flag = 0;
		moveTo(x, y - 1);
	}
	else if (getWorld()->withinBoulder(x, y-1,1))
	{
		setDead();
	}
	else if (y == 0)
	{
		setDead();
	}
	else
		moveTo(x, y - 1);
	if (getWorld()->withinBoulder(getWorld()->getFrackMan()->getX(), getWorld()->getFrackMan()->getY(), 0))
	{
		getWorld()->getFrackMan()->setDead();
	}
	getWorld()->hitprotestor(this);
}
Protestor::Protestor(int startX, int startY, int health, StudentWorld* World)
	:objectbase(IID_PROTESTER, startX, startY, left, 1.0, 0, World, true),m_state(rest),m_count(0),s_count(0),inter_count(1)
{
	numSquaresToMoveInCurrentDirection = getWorld()->randInt(8, 60);
	setHealth(health);
	N = max(50, 100 - int(getWorld()->getLevel()) * 10);
	ticksToWaitBetweenMoves = max(0, 3 - int(getWorld()->getLevel() / 4));
	temptick = ticksToWaitBetweenMoves;
}

void Protestor::doSomething()
{
	if (isDead())
		return;
	if (m_count > temptick&&m_state!=leave)
	{
		m_state = non_rest;
		m_count = 0;
		if (temptick == N)
			temptick = ticksToWaitBetweenMoves;
	}
	else if(m_state != leave)
		m_state = rest;
	if (m_state == rest)
	{
		m_count++;
		return;
	}
	else if (m_state == leave)
	{
		if (getX() == 60 && getY() == 60)
			setDead();
		else
		{
			Position leave = m_stack.top();
			int l_x = leave.p_x;
			int l_y = leave.p_y;
			m_stack.pop();
			if (l_x > getX())
				setDirection(GraphObject::right);
			if (l_x < getX())
				setDirection(GraphObject::left);
			if (l_y > getY())
				setDirection(GraphObject::up);
			if (l_y <getY())
				setDirection(GraphObject::down);
			moveTo(leave.p_x, leave.p_y);
		}
		
	}
	else if (m_state == non_rest)
	{
		m_count=0;
		if (inter_count > 0 && inter_count <= 201)
		{
			inter_count++;
		}
		else
			inter_count = 0;
		if (s_count > 0 && s_count <= 16)
			s_count++;
		else
			s_count = 0;
		int p_x = getX();
		int p_y = getY();
		int m_x = getWorld()->getFrackMan()->getX();
		int m_y = getWorld()->getFrackMan()->getY();
		Direction p_dir = getDirection();
		if (s_count==0&&getWorld()->withinUnit(m_x, m_y, 4, this)&& p_x > m_x&&p_dir == left)
		{
			s_count++;
			getWorld()->shout();
			return;
		}
		if (s_count == 0 && getWorld()->withinUnit(m_x, m_y, 4, this)&&p_x < m_x&&p_dir == right)
		{
			s_count++;
			getWorld()->shout();
			return;
		}
		if (s_count == 0 && getWorld()->withinUnit(m_x, m_y, 4, this)&&p_y < m_y&&p_dir == up)
		{
			s_count++;
			getWorld()->shout();
			return;
		}
		if (s_count == 0 && getWorld()->withinUnit(m_x, m_y, 4, this)&& p_y > m_y&&p_dir == down)
		{
			s_count++;
			getWorld()->shout();
			return;
		}
		if (p_y == m_y)
		{
			int flag_t = 0;
			for (int i = min(p_x, m_x) + 1; i < max(p_x, m_x); i++)
				if (getWorld()->occuppied(i,p_y)||getWorld()->withinBoulder(i, p_y,0))
					flag_t++;
			if (flag_t == 0)
			{
				setDirection(p_x > m_x ? left : right);
				if (getDirection() == left)
					moveTo(p_x - 1, p_y);
				if (getDirection() == right)
					moveTo(p_x + 1, p_y);
				numSquaresToMoveInCurrentDirection = 0;
				return;
			}

		}
		if (p_x == m_x)
		{
			int flag_t = 0;
			for (int i = min(p_y, m_y) + 1; i < max(p_y, m_y); i++)
				if (getWorld()->occuppied(m_x, i))
					flag_t++;
			if (flag_t == 0)
			{
				setDirection(p_y > m_y ? down : up);
				if (getDirection() == down)
					moveTo(p_x, p_y-1);
				if (getDirection() == up)
					moveTo(p_x, p_y+1);
				numSquaresToMoveInCurrentDirection = 0;
				return;
			}

		}
		if (numSquaresToMoveInCurrentDirection > 0)
			numSquaresToMoveInCurrentDirection--;
		if (numSquaresToMoveInCurrentDirection == 0)
		{
			setDirection(pickDirection());
			numSquaresToMoveInCurrentDirection = getWorld()->randInt(8, 60);
		}
		setDirection(intersection(p_x, p_y, getDirection()));
		protestorMove(p_x,p_y);
	}
}
void Protestor::protestorMove(int x, int y)
{
	Direction dir_t = getDirection();
	switch(dir_t)
	{
	case(left) :
		if (getWorld()->withinView(x - 1, y) && !getWorld()->occuppied(x - 1, y) && !getWorld()->withinBoulder(x - 1, y, 0))
			moveTo(x - 1, y);
		else
			numSquaresToMoveInCurrentDirection = 0;
		break;
	case(right) :
		if (getWorld()->withinView(x + 1, y) && !getWorld()->occuppied(x + 1, y) && !getWorld()->withinBoulder(x + 1, y, 0))
			moveTo(x + 1, y);
		else
			numSquaresToMoveInCurrentDirection = 0;
		break;
	case(up) :
		if (getWorld()->withinView(x, y + 1) && !getWorld()->occuppied(x, y + 1) && !getWorld()->withinBoulder(x, y + 1, 0))
			moveTo(x, y + 1);
		else
			numSquaresToMoveInCurrentDirection = 0;
		break;
	case(down) :
		if (getWorld()->withinView(x, y - 1) && !getWorld()->occuppied(x, y - 1) && !getWorld()->withinBoulder(x, y + 1, 0))
			moveTo(x, y - 1);
		else
			numSquaresToMoveInCurrentDirection = 0;
		break;
	}
}
GraphObject::Direction Protestor::intersection(int x, int y, Direction dir)
{
	if (inter_count != 0)
		return getDirection();
	if (isIntersection(x, y, dir))
	{
		numSquaresToMoveInCurrentDirection = getWorld()->randInt(8, 60);
		inter_count++;
		int ranD = getWorld()->randInt(1, 2);
		if (dir == left || dir == right)
		{
			if (ranD == 1 && getWorld()->withinView(x, y + 1) && !getWorld()->occuppied(x, y + 1) && !getWorld()->withinBoulder(x, y + 1, 0))
				return up;
			else if (ranD != 2)
				return down;
			if (ranD == 2 && getWorld()->withinView(x, y - 1) && !getWorld()->occuppied(x, y - 1) && !getWorld()->withinBoulder(x, y + 1, 0))
				return down;
			else
				return up;
		}
		if (dir == up || dir == down)
		{
			if (ranD == 1 && getWorld()->withinView(x + 1, y) && !getWorld()->occuppied(x + 1, y) && !getWorld()->withinBoulder(x + 1, y, 0))
				return right;
			else if(ranD!=2)
				return left;
			if (ranD == 2 && getWorld()->withinView(x - 1, y) && !getWorld()->occuppied(x - 1, y) && !getWorld()->withinBoulder(x - 1, y, 0))
				return left;
			else
				return right;
		}
		
	}
	return getDirection();
}
bool Protestor::isIntersection(int x, int y, Direction dir)
{
	if (dir == left || dir == right)
	{
		if (getWorld()->withinView(x, y + 1) && !getWorld()->occuppied(x, y + 1) && !getWorld()->withinBoulder(x, y + 1, 0))
			return true;
		if (getWorld()->withinView(x, y - 1) && !getWorld()->occuppied(x, y - 1) && !getWorld()->withinBoulder(x, y + 1, 0))
			return true;
	}
	if (dir == up || dir == down)
	{
		if (getWorld()->withinView(x + 1, y) && !getWorld()->occuppied(x + 1, y) && !getWorld()->withinBoulder(x + 1, y, 0))
			return true;
		if (getWorld()->withinView(x - 1, y) && !getWorld()->occuppied(x - 1, y) && !getWorld()->withinBoulder(x - 1, y, 0))
			return true;
	}
	return false;
}
bool Protestor::notblock(int x, int y,Direction dir)
{
	if (dir == left && !getWorld()->occuppied(x - 1, y) && !getWorld()->withinBoulder(x - 1, y, 0) && getWorld()->withinView(x - 1, y))
		return true;
	if (dir == right && !getWorld()->occuppied(x + 1, y) && !getWorld()->withinBoulder(x + 1, y, 0) && getWorld()->withinView(x + 1, y))
		return true;
	if (dir == up && !getWorld()->occuppied(x, y + 1) && !getWorld()->withinBoulder(x, y + 1, 0) && getWorld()->withinView(x, y + 1))
		return true;
	if (dir == down && !getWorld()->occuppied(x, y - 1) && !getWorld()->withinBoulder(x, y - 1, 0) && getWorld()->withinView(x, y - 1))
		return true;
	return false;
}

GraphObject::Direction Protestor::pickDirection()
{
    Direction dir=getWorld()->ranDirection();
	while (!notblock(getX(), getY(), dir))
		 dir= getWorld()->ranDirection();
	return dir;
}
void Protestor::decHealth(int hit)
{
	objectbase::decHealth(hit);
	temptick = N;
	if (getHealth() <= 0)
	{
		getWorld()->increaseScore(100);
		getWorld()->figureLeave(this);
		m_state = leave;
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
	}
}

HardcoreProtestors::HardcoreProtestors(int startX, int startY, int health, StudentWorld* World)
	:objectbase(IID_HARD_CORE_PROTESTER, startX, startY, left, 1.0, 0, World, true), m_state(rest), m_count(0), s_count(0), inter_count(1)

{

	numSquaresToMoveInCurrentDirection = getWorld()->randInt(8, 60);
	setHealth(health);
	N = max(50, 100 - int(getWorld()->getLevel()) * 10);
	ticksToWaitBetweenMoves = max(0, 3 - int(getWorld()->getLevel() / 4));
	temptick = ticksToWaitBetweenMoves;
}
void HardcoreProtestors::doSomething()
{
	if (isDead())
		return;
	if (m_count > temptick&&m_state != leave)
	{
		m_state = non_rest;
		m_count = 0;
		if (temptick == N)
			temptick = ticksToWaitBetweenMoves;
	}
	else if (m_state != leave)
		m_state = rest;
	if (m_state == rest)
	{
		m_count++;
		return;
	}
	else if (m_state == leave)
	{
		if (getX() == 60 && getY() == 60)
			setDead();
		else
		{
			Position leave = m_stack.top();
			int l_x = leave.p_x;
			int l_y = leave.p_y;
			m_stack.pop();
			if (l_x > getX())
				setDirection(GraphObject::right);
			if (l_x < getX())
				setDirection(GraphObject::left);
			if (l_y > getY())
				setDirection(GraphObject::up);
			if (l_y <getY())
				setDirection(GraphObject::down);
			moveTo(leave.p_x, leave.p_y);
		}

	}
	else if (m_state == non_rest)
	{
		m_count = 0;
		if (inter_count > 0 && inter_count <= 201)
		{
			inter_count++;
		}
		else
			inter_count = 0;
		if (s_count > 0 && s_count <= 16)
			s_count++;
		else
			s_count = 0;
		int p_x = getX();
		int p_y = getY();
		int m_x = getWorld()->getFrackMan()->getX();
		int m_y = getWorld()->getFrackMan()->getY();
		Direction p_dir = getDirection();
		if (s_count == 0 && getWorld()->withinUnit(m_x, m_y, 4, this) && p_x > m_x&&p_dir == left)
		{
			s_count++;
			getWorld()->shout();
			return;
		}
		if (s_count == 0 && getWorld()->withinUnit(m_x, m_y, 4, this) && p_x < m_x&&p_dir == right)
		{
			s_count++;
			getWorld()->shout();
			return;
		}
		if (s_count == 0 && getWorld()->withinUnit(m_x, m_y, 4, this) && p_y < m_y&&p_dir == up)
		{
			s_count++;
			getWorld()->shout();
			return;
		}
		if (s_count == 0 && getWorld()->withinUnit(m_x, m_y, 4, this) && p_y > m_y&&p_dir == down)
		{
			s_count++;
			getWorld()->shout();
			return;
		}
		if (p_y == m_y)
		{
			int flag_t = 0;
			for (int i = min(p_x, m_x) + 1; i < max(p_x, m_x); i++)
				if (getWorld()->occuppied(i, p_y) || getWorld()->withinBoulder(i, p_y, 0))
					flag_t++;
			if (flag_t == 0)
			{
				setDirection(p_x > m_x ? left : right);
				if (getDirection() == left)
					moveTo(p_x - 1, p_y);
				if (getDirection() == right)
					moveTo(p_x + 1, p_y);
				numSquaresToMoveInCurrentDirection = 0;
				return;
			}

		}
		if (p_x == m_x)
		{
			int flag_t = 0;
			for (int i = min(p_y, m_y) + 1; i < max(p_y, m_y); i++)
				if (getWorld()->occuppied(m_x, i))
					flag_t++;
			if (flag_t == 0)
			{
				setDirection(p_y > m_y ? down : up);
				if (getDirection() == down)
					moveTo(p_x, p_y - 1);
				if (getDirection() == up)
					moveTo(p_x, p_y + 1);
				numSquaresToMoveInCurrentDirection = 0;
				return;
			}

		}
		if (numSquaresToMoveInCurrentDirection > 0)
			numSquaresToMoveInCurrentDirection--;
		if (numSquaresToMoveInCurrentDirection == 0)
		{
			setDirection(pickDirection());
			numSquaresToMoveInCurrentDirection = getWorld()->randInt(8, 60);
		}
		setDirection(intersection(p_x, p_y, getDirection()));
		protestorMove(p_x, p_y);
	}
}
void HardcoreProtestors::protestorMove(int x, int y)
{
	Direction dir_t = getDirection();
	switch (dir_t)
	{
	case(left) :
		if (getWorld()->withinView(x - 1, y) && !getWorld()->occuppied(x - 1, y) && !getWorld()->withinBoulder(x - 1, y, 0))
			moveTo(x - 1, y);
		else
			numSquaresToMoveInCurrentDirection = 0;
		break;
	case(right) :
		if (getWorld()->withinView(x + 1, y) && !getWorld()->occuppied(x + 1, y) && !getWorld()->withinBoulder(x + 1, y, 0))
			moveTo(x + 1, y);
		else
			numSquaresToMoveInCurrentDirection = 0;
		break;
	case(up) :
		if (getWorld()->withinView(x, y + 1) && !getWorld()->occuppied(x, y + 1) && !getWorld()->withinBoulder(x, y + 1, 0))
			moveTo(x, y + 1);
		else
			numSquaresToMoveInCurrentDirection = 0;
		break;
	case(down) :
		if (getWorld()->withinView(x, y - 1) && !getWorld()->occuppied(x, y - 1) && !getWorld()->withinBoulder(x, y + 1, 0))
			moveTo(x, y - 1);
		else
			numSquaresToMoveInCurrentDirection = 0;
		break;
	}
}
GraphObject::Direction HardcoreProtestors::intersection(int x, int y, Direction dir)
{
	if (inter_count != 0)
		return getDirection();
	if (isIntersection(x, y, dir))
	{
		numSquaresToMoveInCurrentDirection = getWorld()->randInt(8, 60);
		inter_count++;
		int ranD = getWorld()->randInt(1, 2);
		if (dir == left || dir == right)
		{
			if (ranD == 1 && getWorld()->withinView(x, y + 1) && !getWorld()->occuppied(x, y + 1) && !getWorld()->withinBoulder(x, y + 1, 0))
				return up;
			else if (ranD != 2)
				return down;
			if (ranD == 2 && getWorld()->withinView(x, y - 1) && !getWorld()->occuppied(x, y - 1) && !getWorld()->withinBoulder(x, y + 1, 0))
				return down;
			else
				return up;
		}
		if (dir == up || dir == down)
		{
			if (ranD == 1 && getWorld()->withinView(x + 1, y) && !getWorld()->occuppied(x + 1, y) && !getWorld()->withinBoulder(x + 1, y, 0))
				return right;
			else if (ranD != 2)
				return left;
			if (ranD == 2 && getWorld()->withinView(x - 1, y) && !getWorld()->occuppied(x - 1, y) && !getWorld()->withinBoulder(x - 1, y, 0))
				return left;
			else
				return right;
		}

	}
	return getDirection();
}
bool HardcoreProtestors::isIntersection(int x, int y, Direction dir)
{
	if (dir == left || dir == right)
	{
		if (getWorld()->withinView(x, y + 1) && !getWorld()->occuppied(x, y + 1) && !getWorld()->withinBoulder(x, y + 1, 0))
			return true;
		if (getWorld()->withinView(x, y - 1) && !getWorld()->occuppied(x, y - 1) && !getWorld()->withinBoulder(x, y + 1, 0))
			return true;
	}
	if (dir == up || dir == down)
	{
		if (getWorld()->withinView(x + 1, y) && !getWorld()->occuppied(x + 1, y) && !getWorld()->withinBoulder(x + 1, y, 0))
			return true;
		if (getWorld()->withinView(x - 1, y) && !getWorld()->occuppied(x - 1, y) && !getWorld()->withinBoulder(x - 1, y, 0))
			return true;
	}
	return false;
}
bool HardcoreProtestors::notblock(int x, int y, Direction dir)
{
	if (dir == left && !getWorld()->occuppied(x - 1, y) && !getWorld()->withinBoulder(x - 1, y, 0) && getWorld()->withinView(x - 1, y))
		return true;
	if (dir == right && !getWorld()->occuppied(x + 1, y) && !getWorld()->withinBoulder(x + 1, y, 0) && getWorld()->withinView(x + 1, y))
		return true;
	if (dir == up && !getWorld()->occuppied(x, y + 1) && !getWorld()->withinBoulder(x, y + 1, 0) && getWorld()->withinView(x, y + 1))
		return true;
	if (dir == down && !getWorld()->occuppied(x, y - 1) && !getWorld()->withinBoulder(x, y - 1, 0) && getWorld()->withinView(x, y - 1))
		return true;
	return false;
}

GraphObject::Direction HardcoreProtestors::pickDirection()
{
	Direction dir = getWorld()->ranDirection();
	while (!notblock(getX(), getY(), dir))
		dir = getWorld()->ranDirection();
	return dir;
}
void HardcoreProtestors::decHealth(int hit)
{
	objectbase::decHealth(hit);
	temptick = N;
	if (getHealth() <= 0)
	{
		getWorld()->increaseScore(250);
		getWorld()->figureLeave(this);
		m_state = leave;
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
	}
}

Barrel::Barrel(int startX, int startY, StudentWorld* World)
	:  objectbase(IID_BARREL, startX, startY, right, 1.0, 2, World, false),m_visible(false)

{

}
void Barrel::doSomething()
{
	if (isDead())
	{
		return;
	}
	int x = getX();
	int y = getY();
	if (m_visible == false)
	{
		if (getWorld()->getRadius(x, y, getWorld()->getFrackMan()) <= 4)
		{
			m_visible = true;
			setVisible(m_visible);
			return;
		}
	}
	if (getWorld()->getRadius(x, y, getWorld()->getFrackMan()) <= 3)
	{
		getWorld()->increaseScore(1000);
		setDead();
		getWorld()->getFrackMan()->decBarrel();
		getWorld()->playSound(SOUND_FOUND_OIL);
		return;
	}
}
GoldNugget::GoldNugget(int startX, int startY, StudentWorld* World)
	: objectbase(IID_GOLD, startX, startY, right, 1.0, 2, World, false), m_visible(false)
{}
void GoldNugget::doSomething()
{
	if (isDead())
		return;
	int x = getX();
	int y = getY();
	if (m_visible == false)
	{
		if (getWorld()->getRadius(x, y, getWorld()->getFrackMan()) <= 4)
		{
			getWorld()->increaseScore(10);
			m_visible = true;
			setVisible(m_visible);
			return;
		}
	}
	if (getWorld()->getRadius(x, y, getWorld()->getFrackMan()) <= 3)
	{
		setDead();
		getWorld()->getFrackMan()->addGold();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		return;
	}
}
Bribe::Bribe(int startX, int startY, StudentWorld* World)
	: objectbase(IID_GOLD, startX, startY, right, 1.0, 2, World, true),m_count(0)
{

}
void Bribe::doSomething()
{
	if (isDead())
		return;
	m_count++;
	if (m_count > 100)
	{
		setDead();
		return;
	}
	getWorld()->gotBribe(this);
	

}
Sonar::Sonar(int startX, int startY, StudentWorld* World)
	: objectbase(IID_SONAR, startX, startY, right, 1.0, 2, World, true), m_count(0)
{
	T = max(100, int(300 - 10 * getWorld()->getLevel()));
}
void Sonar::doSomething()
{
	if (isDead())
		return;
	int x = getX();
	int y = getY();
	m_count++;
	if (m_count > T)
		setDead();
	if (getWorld()->getRadius(x, y, getWorld()->getFrackMan()) <= 3)
	{
		getWorld()->increaseScore(75);
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->getFrackMan()->incSonar();
	}
}
Waterpool::Waterpool(int startX, int startY, StudentWorld* World)
	: objectbase(IID_WATER_POOL, startX, startY, right, 1.0, 2, World, true), m_count(0)
{
	T = max(100, int(300 - 10 * getWorld()->getLevel()));
}
void Waterpool::doSomething()
{
	if (isDead())
		return;
	int x = getX();
	int y = getY();
	m_count++;
	if (m_count > T)
		setDead();
	if (getWorld()->getRadius(x, y, getWorld()->getFrackMan()) <= 3)
	{
		getWorld()->increaseScore(100);
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->getFrackMan()->addSquirt();
	}
}
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
