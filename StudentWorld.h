#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include"Actor.h"
#include <vector>
#include <stack>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

const int Dirt_X = 63;
const int Dirt_Y = 59;
const int Frack_S = 60;
const int shaft_Y_U = 59;
const int shaft_Y_D = 4;
const int shaft_X_L = 30;
const int shaft_X_R = 33;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir),p_T(0),p_N(0)
	{
		test = 0;
	}
	
	virtual int init();

	virtual int move();

	virtual void cleanUp();
	bool withinView(int x, int y);
	bool occuppied(int x, int y);
	FrackMan* getFrackMan();
	Dirt* getDirt(int x, int y);
	void removeDirt(int x, int y);
	void fireSquirt();
	bool allDirt(int x, int y);
	double StudentWorld::getRadius(int x, int y, objectbase*b);
	std::vector<objectbase*> getActor_base();
	bool withinBoulder(int x, int y,int thre);
	bool withinUnit(int x, int y, int unit, objectbase* actor);
	void shout();
	void squirtHit(objectbase* squirt);
	GraphObject::Direction ranDirection();
	int randInt(int min, int max);
	void fireSonar();
	void genMaze();
	void sovleMaze(int sx, int sy);
	void figureLeave(objectbase* P);
	void giveBribe();
	void hitprotestor(objectbase*P);
	void gotBribe(objectbase* bribe_t);
	virtual ~StudentWorld();
private:
	Dirt* m_Dirt[Dirt_X+1][Dirt_Y+1];
	FrackMan* m_man;
	void setDisplayText();
	int test;
	int p_T;
	int p_N;
	std::vector<objectbase*> actor_base;
	int maze[VIEW_HEIGHT][VIEW_WIDTH];
	struct Position
	{
		int p_x, p_y;

		Position() {}
		Position(int X, int Y)
			: p_x(X), p_y(Y) {}
	};
	std::stack<Position> path_o;
	std::stack<Position> path_l;
	bool isCanGo(int preValue, int x, int y);
	void shortestPath(std::stack<Position> &path, Position &start, Position &end);
};
inline
FrackMan* StudentWorld::getFrackMan()
{
	return m_man;
}

inline
Dirt* StudentWorld::getDirt(int x, int y)
{
	return m_Dirt[x][y];
}
inline
std::vector<objectbase*> StudentWorld::getActor_base()
{
	return actor_base;
}
#endif // STUDENTWORLD_H_
