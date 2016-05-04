#ifndef ACTOR_H_
#define ACTOR_H_
#include <stack>
#include "GraphObject.h"
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
const int waiting = 0;
const int stable = 1;
const int falling = 2;

const int rest = 0;
const int leave = 1;
const int non_rest = 2;
class StudentWorld;
class objectbase : public GraphObject
{
public:
	objectbase(int imageID, int startX, int startY, Direction dir , double size , unsigned int depth, StudentWorld* Wolrd, bool visible);
	virtual void doSomething() = 0;
	virtual void pushstack(int x, int y) {}
	int getHealth();
	virtual void decHealth(int hit);
	bool isDead();
	void setDead();
	void setHealth(int health);
	StudentWorld* getWorld();
	virtual ~objectbase(){}
private:
	int m_health;
	bool Dead;
	StudentWorld* m_world;
	int m_ID;
};
class Dirt : public objectbase
{
public:
	Dirt(int startX, int startY, StudentWorld* Wolrd);
	virtual void doSomething();

	virtual ~Dirt(){}
};
class FrackMan : public objectbase
{
public:
	FrackMan(int startX, int startY,int oil, StudentWorld* Wolrd);
	virtual void doSomething();
	int getHealthMan();
	virtual ~FrackMan(){}
	int getSquirt();
	int getleftBarrel();
	void decBarrel();
	void decSquirt();
	void addSquirt();
	void decGold();
	void addGold();
	int getGold();
	int getSonar();
	void incSonar();
	void decSonar();
private:
	int m_gold;
	int m_squirt;
	int m_leftBarrel;
	int m_sonar;
};

class GoldNugget :public objectbase
{
public:
	GoldNugget(int startX, int startY, StudentWorld* World);
	virtual void doSomething();
	virtual ~GoldNugget(){}
private:
	bool m_visible;

};
class Bribe :public objectbase
{
public:
	Bribe(int startX, int startY, StudentWorld* World);
	virtual void doSomething();
	virtual ~Bribe(){}
private:
	int	m_count;
};
class Barrel :public objectbase
{
public:
	Barrel(int startX, int startY, StudentWorld* World);
	virtual ~Barrel(){}
	virtual void doSomething();
private:
	bool m_visible;

};
class Sonar :public objectbase
{
public:
	Sonar(int startX, int startY, StudentWorld* World);
	void doSomething();
	virtual ~Sonar(){}
private:
	int T;
	int m_count;
};
class Squirt :public objectbase
{
public:
	Squirt(int startX, int startY, Direction dir, StudentWorld* World);
	virtual void doSomething();
	virtual ~Squirt() {}
private:
	int m_count=0;
};
class Waterpool :public objectbase
{
public:
	Waterpool(int startX, int startY, StudentWorld* World);
	virtual void doSomething();
	virtual ~Waterpool() {}
private:
	int m_count = 0;
	int T;
};
class Boulder :public objectbase
{
public:
	Boulder(int startX, int startY, StudentWorld* Wolrd);
	virtual void doSomething() ;
	void moveBoulder();
	virtual ~Boulder() {}
private:
	int m_state;
	int m_count;
	int m_flag = 1;
};
class Protestor :public objectbase
{
public:
	Protestor(int startX, int startY, int health, StudentWorld* Wolrd);
	virtual void doSomething();
	bool notblock(int x, int y,Direction dir);
	Direction intersection(int x, int y, Direction dir);
	Direction pickDirection();
	bool isIntersection(int x, int y, Direction dir);
	void protestorMove(int x, int y);
	virtual void pushstack(int x, int y)
	{
		m_stack.push(Position(x, y));
	}
	virtual void decHealth(int hit);
	virtual ~Protestor() {};
private:
	struct Position
	{
		int p_x, p_y;

		Position() {}
		Position(int X, int Y)
			: p_x(X), p_y(Y) {}
	};
	std::stack<Position> m_stack;
	int m_state;
	int m_count;
	int s_count;
	int inter_count;
	int N;
	int numSquaresToMoveInCurrentDirection;
	int temptick;
	int ticksToWaitBetweenMoves;

};
class HardcoreProtestors : public objectbase
{
public:
	HardcoreProtestors(int startX, int startY, int health, StudentWorld* Wolrd);
	virtual void doSomething();
	bool notblock(int x, int y, Direction dir);
	Direction intersection(int x, int y, Direction dir);
	Direction pickDirection();
	bool isIntersection(int x, int y, Direction dir);
	void protestorMove(int x, int y);
	virtual void pushstack(int x, int y)
	{
		m_stack.push(Position(x, y));
	}
	virtual void decHealth(int hit);
	virtual ~HardcoreProtestors(){}
private:
	struct Position
	{
		int p_x, p_y;

		Position() {}
		Position(int X, int Y)
			: p_x(X), p_y(Y) {}
	};
	std::stack<Position> m_stack;
	int m_state;
	int m_count;
	int s_count;
	int inter_count;
	int N;
	int numSquaresToMoveInCurrentDirection;
	int temptick;
	int ticksToWaitBetweenMoves;

};
inline
int FrackMan::getleftBarrel()
{
	return m_leftBarrel;
}
#endif // ACTOR_H_