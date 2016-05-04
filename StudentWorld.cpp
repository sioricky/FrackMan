#include "StudentWorld.h"
#include <string>
#include "Actor.h"
#include <random>
#include<algorithm>
#include<cmath>
using namespace std;
GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::randInt(int min, int max)
{
	if (max < min)
		swap(max, min);
	static random_device rd;
	static mt19937 generator(rd());
	uniform_int_distribution<> distro(min, max);
	return distro(generator);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
StudentWorld::~StudentWorld()
{
	for (int x = 0; x <= Dirt_X; x++)
		for (int y = 0; y <= Dirt_Y; y++)
			delete m_Dirt[x][y];
	delete m_man;
	vector<objectbase*>::iterator it;
	it = actor_base.begin();
	while (it != actor_base.end())
	{
		vector<objectbase*>::iterator temp=it;
		it++;
		delete *temp;

	}
}
int StudentWorld::init() {
	for (int x = 0; x <= Dirt_X; x++)
		for (int y = 0; y <= Dirt_Y; y++)
		{
			m_Dirt[x][y] = new Dirt(x, y,this);
			if (x >= shaft_X_L&&x <= shaft_X_R&&y >= shaft_Y_D&&y <= shaft_Y_U)
				m_Dirt[x][y]->setDead();
		}
	for (int i = 0; i < min(int(getLevel())/2+2, 6);i++)
	{ 
		vector<objectbase*>::iterator it;
		it = actor_base.begin();
		int x = randInt(20, Dirt_X - 3);
		int y = randInt(20, Dirt_Y - 3);
		while (!allDirt(x, y))
		{
			x = randInt(20, Dirt_X - 3);
			y = randInt(20, Dirt_Y - 3);
			while (it != actor_base.end())
			{
				if (getRadius(x, y, (*it))>6)
					break;
			}
			if (it == actor_base.end()&& allDirt(x, y))
				break;
		}
		removeDirt(x, y);
		actor_base.push_back(new Boulder(x, y, this));
	}
	int G = max(5 - int(getLevel()) / 2, 2);
	for (int i = 0; i < G; i++)
	{
		vector<objectbase*>::iterator it;
		it = actor_base.begin();
		int x = randInt(20, Dirt_X - 3);
		int y = randInt(20, Dirt_Y - 3);
		while (!allDirt(x, y))
		{
			x = randInt(20, Dirt_X - 3);
			y = randInt(20, Dirt_Y - 3);
			while (it != actor_base.end())
			{
				if (getRadius(x, y, (*it))>6)
					break;
			}
			if (it == actor_base.end() && allDirt(x, y))
				break;
		}
		actor_base.push_back(new GoldNugget(x, y, this));
	}
	int L = min(int(getLevel()) + 2, 20);
	for (int i = 0; i < L; i++)
	{
		vector<objectbase*>::iterator it;
		it = actor_base.begin();
		int x = randInt(20, Dirt_X - 3);
		int y = randInt(20, Dirt_Y - 3);
		while (!allDirt(x, y))
		{
			x = randInt(20, Dirt_X - 3);
			y = randInt(20, Dirt_Y - 3);
			while (it != actor_base.end())
			{
				if (getRadius(x, y, (*it))>6)
					break;
			}
			if (it == actor_base.end() && allDirt(x, y))
				break;
		}
		actor_base.push_back(new Barrel(x, y, this));
	}
	m_man = new FrackMan(30,60, L,this);
	return GWSTATUS_CONTINUE_GAME;

}

int StudentWorld::move()
{
	// Update the Game Status Line
	setDisplayText(); // update the score/lives/level text at screen top
						 // The term ¡°Actors¡± refers to all Protesters, the player, Goodies,
						 // Boulders, Barrels of oil, Holes, Squirts, the Exit, etc.
						 // Give each Actor a chance to do something
	int T = max(25, int(200 - getLevel()));
	int P = min(15, 2 + int(getLevel() * 1.5));
	int probabilityOfHardcore = min(90, int(getLevel()) * 10 + 30);
	int proba = randInt(1, 100);

	if (p_N == 0)
	{
		if (proba > probabilityOfHardcore)
			actor_base.push_back(new Protestor(60, 60, 5, this));
		else
			actor_base.push_back(new HardcoreProtestors(60, 60, 20, this));
		p_N++;
	}
	p_T++;
	if (p_T >T )
	{
		p_T = 0;
		if (p_N <= P)
		{
			if (proba > probabilityOfHardcore)
				actor_base.push_back(new Protestor(60, 60, 5, this));
			else
				actor_base.push_back(new HardcoreProtestors(60, 60, 20, this));
			p_N++;
		}
	}
	int G = getLevel() * 25 + 300;
	int ghappen = randInt(1, G);
	int fhappen = randInt(1, 5);
	if (ghappen == 1 && fhappen == 1)
	{
		actor_base.push_back(new Sonar(0, 60, this));
	}
	if (ghappen == 1 && fhappen > 1)
	{

		int x_w=randInt(0,VIEW_WIDTH-4);
		int y_w = randInt(0, VIEW_HEIGHT-4);
		while (occuppied(x_w, y_w))
		{
			x_w = randInt(0, VIEW_WIDTH-4);
			y_w = randInt(0, VIEW_HEIGHT-4);
		}
			actor_base.push_back(new Waterpool(x_w, y_w, this));

	}
	m_man->doSomething();
	vector<objectbase*>::iterator it;
	it = actor_base.begin();
	while (it != actor_base.end())
	{
		(*it)->doSomething();
		it++;
	}
	for (int i = 0; i < actor_base.size(); i++)
	{
		if (actor_base[i]->isDead())
		{
			delete actor_base[i];
			actor_base.erase(actor_base.begin() + i);
			i--;
			break;
		}
	}
	if (m_man->getleftBarrel() == 0)
	{
		return GWSTATUS_FINISHED_LEVEL;
	}
    if (m_man->isDead() == true)
	{ 
//		decLives();
		playSound(SOUND_PLAYER_ANNOYED);
		return GWSTATUS_PLAYER_DIED;

	}

// the player hasn¡¯t completed the current level and hasn¡¯t died
// let them continue playing the current level

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	p_N = 0;
	p_T = 0;
	for (int x = 0; x <= Dirt_X; x++)
		for (int y = 0; y <= Dirt_Y; y++)
			delete m_Dirt[x][y];
	delete m_man;
	for (int i = 0; i < actor_base.size(); i++)
	{
			delete actor_base[i];
			actor_base.erase(actor_base.begin() + i);
			i--;
		}

}

bool StudentWorld::withinView(int x, int y)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (x + i < 0 || x + i >= VIEW_WIDTH || y + j < 0 || y + j >= VIEW_HEIGHT)
				return false;
	return true;
}


bool StudentWorld::allDirt(int x, int y)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (m_Dirt[x + i][y + j]->isDead())
				return false;
	return true;
}
bool StudentWorld::occuppied(int x, int y )
{
	if (x > Dirt_X || y > Dirt_Y||x<0||y<0)
		return false;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			if (x+i> Dirt_X || y+j> Dirt_Y||x<0||y<0)
				break;
			else if (!m_Dirt[x + i][y + j]->isDead())
				return true;
		}

	return false;
}
void StudentWorld::removeDirt(int x, int y)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			if (x + i> Dirt_X || y + j> Dirt_Y)
				break;
			else if (!m_Dirt[x + i][y + j]->isDead())
				m_Dirt[x + i][y + j]->setDead();
		}
			
}

void StudentWorld::fireSquirt()
{
	playSound(SOUND_PLAYER_SQUIRT);
	GraphObject::Direction dir;
	dir=m_man->getDirection();
	if (m_man->getSquirt() != 0)
	{		
		int x_m;
		int y_m;
		GraphObject::Direction Dir;
		switch (dir)
		{
		case GraphObject::up:
			x_m = m_man->getX();
			y_m = m_man->getY() + 4;
			Dir = GraphObject::up;
			break;
		case GraphObject::down:
			x_m = m_man->getX();
			y_m = m_man->getY() - 4;
			Dir = GraphObject::down;
			break;
		case GraphObject::left:
			x_m = m_man->getX() - 4;
			y_m = m_man->getY();
			Dir = GraphObject::left;
			break;
		case GraphObject::right:
			x_m = m_man->getX() + 4;
			y_m = m_man->getY();
			Dir = GraphObject::right;
			break;
		}
		actor_base.push_back(new Squirt(x_m, y_m, dir, this));
		m_man->decSquirt();
	}
}

double StudentWorld::getRadius(int x, int y, objectbase*b)
{
	int a_x = x;
	int a_y = y;
	int b_x = b->getX();
	int b_y = b->getY();
	return	sqrt(pow(a_x - b_x, 2) + pow(a_y - b_y, 2));
}

bool StudentWorld::withinBoulder(int x, int y,int thre)
{
	vector<objectbase*>::iterator it;
	it = actor_base.begin();
	while (it != actor_base.end())
	{
		if ((*it)->getID() == IID_BOULDER&&getRadius(x, y, *(it)) <= 3 && getRadius(x, y, *(it)) > thre)
			return true;
		it++;
	}
	return false;
}
bool StudentWorld::withinUnit(int x, int y, int unit, objectbase* actor)
{
	int a_x = actor->getX();
	int a_y = actor->getY();
	int d_x = abs(x - a_x);
	int d_y = abs(y - a_y);
	if (d_x <= unit&& d_y<= unit)
	{
		return true;
	}
	return false;
}
void StudentWorld::shout()
{
	playSound(SOUND_PROTESTER_YELL);
	m_man->decHealth(2);
}
GraphObject::Direction StudentWorld::ranDirection()
{
	int i = randInt(1, 4);
		switch (i)
		{
		case(1) :
			return GraphObject::left;
			break;
		case(2) :
			return GraphObject::right;
			break;
		case(3) :
			return GraphObject::up;
			break;
		case(4) :
			return GraphObject::down;
			break;
		}
}
void StudentWorld::squirtHit(objectbase* squirt)
{
	int x_s = squirt->getX();
	int y_s = squirt->getY();
	vector<objectbase*>::iterator it_t;
	it_t = actor_base.begin();
	while (it_t != actor_base.end())
	{
		if ((*it_t)->getID() == IID_PROTESTER || (*it_t)->getID() == IID_HARD_CORE_PROTESTER)
		{
			if (getRadius(x_s, y_s, (*it_t))<=3)
			{
				squirt->setDead();
				(*it_t)->decHealth(2);
				playSound(SOUND_PROTESTER_ANNOYED);
			}
		}
		it_t++;
	}
}
void StudentWorld::genMaze()
{
/*	for (int i = 0; i < Dirt_X; i++)
		for (int j = 0; j < Dirt_Y; j++)
		{
			if (!m_Dirt[i][j]->isDead())
				maze[i][j] = 0;
			else
				maze[i][j] = 0;
		}
	for (int i = Dirt_X; i < VIEW_WIDTH; i++)
		for (int j = Dirt_Y; j < VIEW_HEIGHT; j++)
			maze[i][j] = 0;
			*/
	for (int i = 0; i < VIEW_WIDTH; i++)
		for (int j = 0; j < VIEW_HEIGHT; j++)
		{
			
				maze[i][j] = 0;
		}
}
void StudentWorld::sovleMaze(int sx, int sy)
{
	int ex = 60;
	int ey = 60;
	genMaze();
	std::stack<Position> path_t;
	shortestPath(path_t, Position(sx, sy), Position(ex, ey));
}
void StudentWorld::figureLeave(objectbase* P)
{
	int sx = P->getX();
	int sy = P->getY();
	sovleMaze(sx, sy);
	while(!path_o.empty())
	{
		Position Position_t = path_o.top();
		P->pushstack(Position_t.p_x, Position_t.p_y);
		path_o.pop();
	}
}
/*void StudentWorld::leaveland(Protestor* P)
{
	Position leave = path_l.top();
	int l_x = leave.p_x;
	int l_y = leave.p_y;
	path_l.pop();
	if (l_x > P->getX())
		P->setDirection(GraphObject::right);
	if (l_x < P->getX())
		P->setDirection(GraphObject::left);
	if (l_y > P->getY())
		P->setDirection(GraphObject::up);
	if (l_y < P->getY())
		P->setDirection(GraphObject::down);
	P->moveTo(leave.p_x, leave.p_y);
}*/

bool StudentWorld::isCanGo(int preValue, int x, int y)
{
	test++;
	if (!withinView(x,y)||maze[x][y] == -1||withinBoulder(x,y,0)||occuppied(x,y))   
	{
		return false;
	}

	if (maze[x][y] == 0)    
		return true;
	else                        
		return (preValue + 1) < maze[x][y];
}
void StudentWorld::shortestPath(stack<Position> &path, Position &start, Position &end)
{
	Position offset[4];
	offset[0].p_x = -1;    offset[0].p_y = 0;   
	offset[1].p_x = 1;    offset[1].p_y = 0;    
	offset[2].p_x = 0;    offset[2].p_y = -1;   
	offset[3].p_x = 0;    offset[3].p_y = 1;    
	if (start.p_x == end.p_x&& start.p_y == end.p_y)
	{
		if (path.size() < path_o.size() || path_o.empty())   
			path_o = path;
		return;
	}

	for (int i = 0; i < 4; i++)
	{
		int nNextPos_X = start.p_x + offset[i].p_x;
		int nNextPos_Y = start.p_y + offset[i].p_y;

		if (isCanGo(maze[start.p_x][start.p_y], nNextPos_X, nNextPos_Y))
		{

			maze[nNextPos_X][nNextPos_Y] = maze[start.p_x][start.p_y] + 1;
			path.push(Position(nNextPos_X, nNextPos_Y));

			shortestPath(path, Position(nNextPos_X, nNextPos_Y), end);

			path.pop();
		}
	}
}
void StudentWorld::giveBribe()
{
	int x = m_man->getX();
	int y = m_man->getY();
	actor_base.push_back(new Bribe(x, y, this));
}
void StudentWorld::gotBribe(objectbase* bribe_t)
{
	int x_b = bribe_t->getX();
	int y_b = bribe_t->getY();
	vector<objectbase*>::iterator it_t;
	it_t = actor_base.begin();
	while (it_t != actor_base.end())
	{
		if ((*it_t)->getID() == IID_PROTESTER )
		{
			if (getRadius(x_b, y_b, (*it_t)) <= 3)
			{
				increaseScore(25);
				bribe_t->setDead();
				(*it_t)->decHealth(5);
				playSound(SOUND_PROTESTER_FOUND_GOLD);
			}
		}
		if( (*it_t)->getID() == IID_HARD_CORE_PROTESTER)
		{
			if (getRadius(x_b, y_b, (*it_t)) <= 3)
			{
				increaseScore(50);
				bribe_t->setDead();
				(*it_t)->decHealth(0);
				playSound(SOUND_PROTESTER_FOUND_GOLD);
			}
		}
		it_t++;
	}
}
void StudentWorld::hitprotestor(objectbase*P)
{
	int x_p = P->getX();
	int y_p= P->getY();
	vector<objectbase*>::iterator it_t;
	it_t = actor_base.begin();
	while (it_t != actor_base.end())
	{
		if ((*it_t)->getID() == IID_PROTESTER)
		{
			if (getRadius(x_p, y_p, (*it_t)) <= 3)
			{
				increaseScore(500);
				(*it_t)->setDead();
				playSound(SOUND_PROTESTER_GIVE_UP);
			}
		}
		if ((*it_t)->getID() == IID_HARD_CORE_PROTESTER)
		{
			if (getRadius(x_p, y_p, (*it_t)) <= 3)
			{
				increaseScore(500);
				(*it_t)->setDead();
				playSound(SOUND_PROTESTER_GIVE_UP);
			}
		}
		it_t++;
	}
}
void StudentWorld::fireSonar()
{
	int x_m = m_man->getX();
	int y_m = m_man->getY();
	vector<objectbase*>::iterator it_t;
	it_t = actor_base.begin();
	while (it_t != actor_base.end())
	{
		if (getRadius(x_m, y_m, (*it_t)) <= 12)
		{
			(*it_t)->setVisible(true);
		}
		it_t++;
	}
}
void StudentWorld::setDisplayText()
{
	int score = getScore();
	int level = getLevel();
	int lives = getLives();
	int health = m_man->getHealthMan();
	int squirts = m_man->getSquirt();
	int gold = m_man->getGold();
	int sonar = m_man->getSonar();
	int barrelsLeft = m_man->getleftBarrel();
	// Next, create a string from your statistics, of the form:
	// ¡°Scr: 0321000 Lvl: 52 Lives: 3 Hlth: 80% Water: 20 Gld: 3 Sonar: 1 Oil Left: 2¡±

//	string s = someFunctionYouUseToFormatThingsNicely(score, level, lives,
//		health, squirts, gold, sonar, barrelsLeft);
	string s;
	s += "Src: " + to_string(score)+" ";
	s += "Lvl: " + to_string(level) + " ";
	s += "Lives: " + to_string(lives) + " ";
	s += "Hlth: " + to_string(health) + "% ";
	s += "Water: " + to_string(squirts) + " ";
	s += "Gld: " + to_string(gold) + " ";
	s += "Sonar: " + to_string(sonar) + " ";
	s += "Oil Left: " + to_string(barrelsLeft) + " ";
	// Finally, update the display text at the top of the screen with your
	// newly created stats
	setGameStatText(s); // calls our provided GameWorld::setGameStatText
}