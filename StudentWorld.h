#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
class Socrates;
class Actor;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void addActor(Actor* a);
	Socrates* getSocrates();
	void setCompleted();
	bool isComplete();
	bool pitEmpty();
	void setPitEmpty();
	bool overlap(double x1, double y1, double x2, double y2);
	bool overlapsWithDirt(int x, int y);
	bool overlapsWithCanBeDamaged(int x, int y, int pointsDamaged);
	int numOfDeadPits();
	void setNumOfDeadPits();
	int numOfDeadBacteria();
	void setNumOfDeadBacteria();
	int numOfBacteria();
	void setNumOfBacteria();
	bool overlapsWithFood(int x, int y);
	bool getCommon(int x, int y, int i, int dis);
	int closeToFood(int x, int y, int& angle);

private:
	Socrates* m_socrates;
	std::vector <Actor*> myActorVector;
	bool m_isComplete;
	bool m_isPitEmpty;
	int m_numOfDeadPits;
	int m_numOfDeadBacteria;
	int m_numOfBacteria;
};

#endif // STUDENTWORLD_H_
