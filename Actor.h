#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(int mid, bool alive, int x, int y, Direction mdirection, int mdepth, StudentWorld* myStudentWorld, int hitPoints);
	virtual void doSomething() = 0;
	void setAlive(bool alive);
	bool getAlive();
	StudentWorld* getStudentWorld() { return m_studentWorld; }
	Direction dir() { return m_direction;}
	virtual ~Actor() {};
	int getHitPoints();
	void updateHitPoints(int points);
	int returnMax(int a, int b);
	bool overlapsWithSocrates(double x, double y);
	virtual bool canBlock() = 0;
	double distanceFromSocrates(int x, int y);
	virtual bool canBeDamaged() = 0;
	int distanceFromCenter(int x, int y, Direction d, int pixels);
	double angleTowardsSocrates(int x, int y);
	int maxx();
	virtual bool canBeEaten();
	int startHitPoints();
	
private:
	bool m_alive;
	int m_numOfBacteria;
	int m_hitPoints;
	Direction m_direction;
	StudentWorld* m_studentWorld;
	int m_maxx;
	int m_startHitPoints;
};

class Socrates : public Actor
{
public:
	Socrates(StudentWorld* myStudentWorld);
	virtual void doSomething();
	void updateFlameThrower(int num);
	int getFlameThrower();
	int getSprayCharges();
	void setSprayCharges(int num);
	virtual ~Socrates() {}
	void movement(Direction d);
	virtual bool canBlock();
	virtual bool canBeDamaged();
private:
	int m_sprayCharges;
	int m_flameThrowerCharges;
	int m_angle;
};

class Dirt : public Actor
{
public:
	Dirt(int dirtX, int dirtY, StudentWorld* myStudentWorld);
	virtual void doSomething() {}
	virtual ~Dirt() {}
	virtual bool canBlock();
	virtual bool canBeDamaged();
private:
};

class Bacteria : public Actor
{
public:
	Bacteria(int mid, bool alive, int bacX, int bacY, Direction dir, int depth, StudentWorld* myStudentWorld, int hitPoints, int movementPlanDistance, int damagePoints);
	virtual void doSomething();
	virtual ~Bacteria() ;
	virtual bool canBlock();
	virtual bool canBeDamaged();
	virtual void doDifferent1(int & movementPlanDistance) = 0;
	virtual void addNewActor(int x, int y) = 0;
	virtual void aSalmonellaDifferent(bool& a) { a = true; };
	
private:
	int m_movementPlanDistance;
	int m_foodEaten;
	int m_damagePoints;
};

class EColi : public Bacteria {
public:
	EColi(int eColiX, int eColiY, StudentWorld* myStudentWorld);
	virtual ~EColi() {}
	virtual void doDifferent1(int& movementPlanDistance);
	virtual void addNewActor(int x, int y);
private:
};

class Salmonella : public Bacteria {
public:
	Salmonella(int mid, int salX, int salY, StudentWorld* myStudentWorld, int hitPoints, int damagePoints);
	virtual ~Salmonella() {}
	virtual void doDifferent1(int& movementPlanDistance);
	virtual void addNewActor(int x, int y) = 0;
private:
};

class RSalmonella : public Salmonella {
public:
	RSalmonella(int rSalX, int rSalY, StudentWorld* myStudentWorld);
	virtual ~RSalmonella() {}
	virtual void addNewActor(int x, int y);
private:
};

class ASalmonella : public Salmonella {
public:
	ASalmonella(int aSalX, int aSalY, StudentWorld* myStudentWorld);
	virtual ~ASalmonella() {}
	virtual void addNewActor(int x, int y);
	virtual void aSalmonellaDifferent(bool& a);
private:
};

class Projectile : public Actor {
public:
	Projectile(int mid, bool alive, int proX, int proY, Direction dir, int depth, StudentWorld* myStudentWorld, int maxTravel, int decAmount);
	virtual void doSomething();
	virtual ~Projectile() {};
	virtual bool canBlock();
	virtual bool canBeDamaged();
private:
	int m_movement;
	int m_maxTravel;
	int m_decAmount;
};

class Flame : public Projectile {
public:
	Flame(int flameX, int flameY, Direction dir, StudentWorld* myStudentWorld);
	virtual ~Flame() {}
private:
};

class Spray : public Projectile {
public:
	Spray(int sprayX, int sprayY, Direction dir, StudentWorld* myStudentWorld);
	virtual ~Spray() {}
private:
};

class Goodie : public Actor {
public:
	Goodie(int mid, bool alive, int gooX, int gooY, Direction dir, int depth, StudentWorld* myStudentWorld, int hitPoints, int updatePoints);
	virtual void doSomething();
	virtual void doDifferent() = 0;
	virtual ~Goodie() {};
	virtual bool canBlock();
	virtual bool canBeDamaged();
private:
	int m_lifetime;
	int m_ticks;
	int m_updatePoints;
};

class RestoreHealth : public Goodie {
public:
	RestoreHealth(int rhX, int rhY, StudentWorld* myStudentWorld);
	virtual void doDifferent();
	virtual ~RestoreHealth() {}
private:
};

class FlameThrower : public Goodie {
public:
	FlameThrower(int flX, int flY, StudentWorld* myStudentWorld);
	virtual void doDifferent();
	virtual ~FlameThrower() {}
private:

};

class ExtraLife : public Goodie {
public:
	ExtraLife(int elX, int elY, StudentWorld* myStudentWorld);
	virtual void doDifferent();
	virtual ~ExtraLife() {};
private:
};

class Pit : public Actor {
public:
	Pit(int pitX, int pitY, StudentWorld* myStudentWorld);
	virtual void doSomething();
	virtual ~Pit() {}
	virtual bool canBlock();
	virtual bool canBeDamaged();
private:
	int m_numOfRSalmonella;
	int m_numOfASalmonella;
	int m_numOfEColi;
};

class Fungus : public Actor {
public:
	Fungus(int fungusX, int fungusY, StudentWorld* myStudentWorld);
	virtual void doSomething();
	virtual ~Fungus() {}
	virtual bool canBlock();
	virtual bool canBeDamaged();
private:
	int m_ticks;
	int m_lifetime;
};

class Food : public Actor {
public:
	Food(int foodX, int foodY, StudentWorld* myStudentWorld);
	virtual void doSomething() {}
	virtual ~Food() {}
	virtual bool canBlock();
	virtual bool canBeDamaged();
	virtual bool canBeEaten();
private:
};


#endif // ACTOR_H_