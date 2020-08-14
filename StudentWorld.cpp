#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"
#include<cmath>
#include <cstdlib>
#include <sstream>
#include<iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	GameWorld* a = new StudentWorld(assetPath);
	return a;
}



// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
	: GameWorld(assetPath)
{
	
}

int StudentWorld::init()
{
	m_isComplete = false;
	m_isPitEmpty = false;
	m_numOfDeadPits = 0;
	m_numOfDeadBacteria = 0;
	m_numOfBacteria = 10*getLevel();


	int level = getLevel();
	bool ableToAdd = true;
	int numOfPits = 0;

	//adding socrates
	m_socrates = new Socrates(this);
	

	//adding pits
	while(numOfPits < getLevel())
	{
		int xd = randInt(0, VIEW_WIDTH);
		int yd = randInt(0, VIEW_WIDTH);

		if (sqrt(pow(xd - VIEW_WIDTH / 2, 2) + pow(yd - VIEW_HEIGHT / 2, 2)) < 120) {
			ableToAdd = true;
		}
		else {
			ableToAdd = false;
			continue;
		}

		for (int i = 0; i < myActorVector.size(); i++)
		{
			if (overlap(xd, yd, myActorVector[i]->getX(), myActorVector[i]->getY()) == true) {
				ableToAdd = false;
				break;
			}
		}

		if (ableToAdd == true) {
			myActorVector.push_back(new Pit(xd, yd, this));
			numOfPits++;
		}
		ableToAdd = true;
	}
	

	// adding food
	int foodObjects = min(5 * level, 25);
	int countOfFood = 0;
	while (countOfFood < foodObjects)
	{
		int xd = randInt(0, VIEW_WIDTH);
		int yd = randInt(0, VIEW_WIDTH);

		if (sqrt(pow(xd - VIEW_WIDTH / 2, 2) + pow(yd - VIEW_HEIGHT / 2, 2)) < 120) {
			ableToAdd = true;
		}
		else {
			ableToAdd = false;
			continue;
		}

		for (int i = 0; i < myActorVector.size(); i++)
		{
			if (overlap(xd, yd, myActorVector[i]->getX(), myActorVector[i]->getY()) == true) {
				ableToAdd = false;
				break;
			}
			
		}

		if (ableToAdd == true) {
		myActorVector.push_back(new Food(xd, yd, this));
		countOfFood++;
		}
		ableToAdd = true;
	}

	//adding dirt
	int numOfActors = myActorVector.size();
	int num = max(180 - 20 * level, 20);
	int countOfDirt = 0;
	while(countOfDirt<num)
	{
		int xd = randInt(0, VIEW_WIDTH);
		int yd = randInt(0, VIEW_WIDTH);

		if (sqrt(pow(xd - VIEW_WIDTH / 2, 2) + pow(yd - VIEW_HEIGHT / 2, 2)) < 120) {
			ableToAdd = true;
		}
		else {
			ableToAdd = false;
			continue;
		}

		for (int i = 0; i < numOfActors; i++)
		{
			if (overlap(xd, yd, myActorVector[i]->getX(), myActorVector[i]->getY()) == true) {
				ableToAdd = false;
				break;
			}
		
		}

		if (ableToAdd == true) {
			myActorVector.push_back(new Dirt(xd, yd, this));
			countOfDirt++;
		}
		ableToAdd = true;
	}

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	int level = getLevel();
	m_socrates->doSomething();
	for (int i = 0; i < myActorVector.size(); i++)
	{
		if(myActorVector[i]->getAlive() == true)
			myActorVector[i]->doSomething();
		if (m_socrates->getAlive() == false)
		{
			decLives();
			//delete m_socrates;
			return GWSTATUS_PLAYER_DIED;
		}
		if (isComplete() == true) { 
			//delete m_socrates;
			playSound(SOUND_FINISHED_LEVEL);
			return GWSTATUS_FINISHED_LEVEL;
		}
	}

	for (int i = 0; i < myActorVector.size(); i++)
	{
		if (myActorVector[i]->getAlive() == false) {
			delete myActorVector[i];
			myActorVector.erase(myActorVector.begin() + i);
		}
	}

	//add new actors to game
	
	//adding fungus to the game
	double PI = 3.14159;
	int chanceFungus = max(510 - level * 10, 200);
	int rando = randInt(0, chanceFungus - 1);
	if (rando == 0) 
	{
		int random = rand()* 360 * VIEW_RADIUS;
		random = (random / 180) * PI;
		int xd = VIEW_RADIUS * cos(random) + 128;
		int yd = VIEW_RADIUS * sin(random) + 128;
		myActorVector.push_back(new Fungus(xd, yd, this));
	}
	
	//adding goodies to the game
	int chanceGoodie = max(510 - getLevel() * 10, 250);
	int rando2 = randInt(0, chanceGoodie - 1);
	if (rando2 == 0)
	{
		int random = rand() * 360 * VIEW_RADIUS;
		random = (random / 180) * PI;
		int xd = VIEW_RADIUS * cos(random) + 128;
		int yd = VIEW_RADIUS * sin(random) + 128;
		
		int rando3 = randInt(1, 10);
		if (rando3 <= 6)
			myActorVector.push_back(new RestoreHealth(xd, yd, this));
		else if (rando3 >6 && rando3 <=9)
			myActorVector.push_back(new FlameThrower(xd, yd, this));
		else if(rando3 == 10)
			myActorVector.push_back(new ExtraLife(xd, yd, this));
	}		

	ostringstream oss;  // --> need to make this right
	oss.fill(' ');
	oss << setw(9);
	oss << "Score: "  ;
	oss.fill('0');
	oss<< setw(6) << getScore() << setw(1) << "  ";
	oss.fill(' ');
	oss << "Level: " << setw(2) << getLevel() << "  ";
	oss << "Lives: " << setw(1) << getLives() << setw(1) << "  ";
	oss << "Health: " << setw(3) << getSocrates()->getHitPoints() << setw(1) << "  ";
	oss << "Sprays: " << setw(1) << getSocrates()->getSprayCharges() << setw(1) << "  ";
	oss << "Flames: " << setw(2) << getSocrates()->getFlameThrower() << setw(1) << "  ";
	string s = oss.str();
	setGameStatText(s);

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete m_socrates;
	m_socrates = nullptr;
	for (int i = 0; i < myActorVector.size();)
	{
		delete myActorVector[i];
		myActorVector.erase(myActorVector.begin());
	}
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}

void StudentWorld::addActor(Actor* a)
{
	myActorVector.push_back(a);
}

Socrates* StudentWorld::getSocrates() {
	return m_socrates;
}

void StudentWorld::setCompleted() {
	m_isComplete = true;
}

bool StudentWorld::isComplete()
{
	if (numOfDeadPits() == getLevel() && numOfDeadBacteria()>= numOfBacteria())
		return true;
	else
		return false;
}

int StudentWorld::numOfDeadBacteria() {
	return m_numOfDeadBacteria;
}

void StudentWorld::setNumOfDeadBacteria()
{
	m_numOfDeadBacteria++;
}

bool StudentWorld::overlap(double x1, double y1, double x2, double y2) {
	double x = x1 - x2;
	double y = y1 - y2;
	double distance = sqrt(pow(x, 2) + pow(y, 2));
	if (distance <= 2*SPRITE_RADIUS)
		return true;
	else
		return false;
}

bool StudentWorld::getCommon(int x, int y, int i, int dis) {

	double xx = abs(myActorVector[i]->getX() - x);
	double yy = abs(myActorVector[i]->getY() - y);
	double distance = sqrt(pow(xx, 2) + pow(yy, 2));
	if (distance <= dis)
		return true;
	else
		return false;
}

bool StudentWorld::overlapsWithDirt(int x, int y)
{
	for (int i = 0; i < myActorVector.size(); i++)
	{
		if (getCommon(x,y, i, SPRITE_RADIUS)){
			if(myActorVector[i]->canBlock())
				return true;
		}
	}
	return false;
}

bool StudentWorld::pitEmpty() {
	return m_isPitEmpty;
}

void StudentWorld::setPitEmpty() {
	m_isPitEmpty = true;
}

int StudentWorld::numOfDeadPits()
{
	return m_numOfDeadPits;
}

void StudentWorld::setNumOfDeadPits()
{
	m_numOfDeadPits++;
}

int StudentWorld::numOfBacteria() {
	return m_numOfBacteria;
}
void StudentWorld::setNumOfBacteria() {
	m_numOfBacteria++;
}

bool StudentWorld::overlapsWithCanBeDamaged(int x, int y, int pointsDamaged) {
	for (int i = 0; i < myActorVector.size(); i++)
	{
		if (getCommon(x,y, i, 2*SPRITE_RADIUS)){ 
			if (myActorVector[i]->canBeDamaged()) {
				if (myActorVector[i]->getHitPoints() == 0) {
					myActorVector[i]->setAlive(false);
					return true;
				}
				myActorVector[i]->updateHitPoints(-pointsDamaged);
				if (myActorVector[i]->getHitPoints() <= 0) {
					myActorVector[i]->setAlive(false);
					if (myActorVector[i]->startHitPoints() == 5)
						playSound(SOUND_ECOLI_DIE);
					else
						playSound(SOUND_SALMONELLA_DIE); 
					increaseScore(100); 
					int rand = randInt(1, 2);
					if (rand == 1)
					{
						int x = myActorVector[i]->getX();
						int y = myActorVector[i]->getY();
						myActorVector.push_back(new Food(x,y,this));
					}
				}
				if (myActorVector[i]->startHitPoints() == 5)
					playSound(SOUND_ECOLI_HURT);
				else
					playSound(SOUND_SALMONELLA_HURT);
				return true;
			}
		}
	}
	return false;
}


bool StudentWorld::overlapsWithFood(int x, int y) {
	for (int i = 0; i < myActorVector.size(); i++)
	{
		if (getCommon(x,y, i, 2*SPRITE_RADIUS)){  
			if (myActorVector[i]->canBeEaten()) {
				myActorVector[i]->setAlive(false);
				return true;
			}
		}
	}
	return false;
}

int StudentWorld::closeToFood(int x, int y, int& angle) {
	int minDistance = 256;
	int ang;
	float PI = 3.14159;
	for (int i = 0; i < myActorVector.size(); i++)
	{
		if (myActorVector[i]->canBeEaten()) {
			double xx = abs(myActorVector[i]->getX() - x);
			double yy = abs(myActorVector[i]->getY() - y);
			double distance = sqrt(pow(xx, 2) + pow(yy, 2));
			if (distance < minDistance)
			{
				double dirX = myActorVector[i]->getX() - x; // getting differences in x and y coordinates
				double dirY = myActorVector[i]->getY() - y;
				ang = atan2(dirY, dirX); // using arctan2 so can get the radians between the two, aournd the whole grid, not just half of it
				ang = ang * (180 / PI);
				minDistance = distance;
			}
		}
	}

	if (minDistance <= 128) {
		angle = ang;
		return angle;
	}
	else
		return angle;
}