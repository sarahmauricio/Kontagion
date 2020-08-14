#include "Actor.h"
#include "StudentWorld.h"
#include <ctime>
#include <cmath>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

/////////////ACTOR IMPLEMENTATIONS/////////////

Actor::Actor(int mid, bool alive, int x, int y, Direction mdirection, int mdepth, StudentWorld* myStudentWorld, int hitPoints) : GraphObject(mid, x, y, mdirection, mdepth, 1)
{
	m_studentWorld = myStudentWorld;
	m_direction = mdirection;
	m_hitPoints = hitPoints;
	m_alive = alive;
	srand(time(0));
	m_maxx = returnMax(rand() % (300 - 10 * getStudentWorld()->getLevel()), 50);
	m_startHitPoints = hitPoints;
}

int Actor:: maxx() { // returns the max time for  a goodie
	return m_maxx;
}

void Actor::setAlive(bool alive) { // sets the dead/alive state for each actor (false is dead)
	m_alive = alive;
}

bool Actor::getAlive() { // returns the dead/alive state for each actor (false is dead)
	return m_alive;
}

int Actor::getHitPoints() { // returns the number of hit points an actor has left
	return m_hitPoints;
}

int Actor::startHitPoints() {
	return m_startHitPoints;
}

void Actor::updateHitPoints(int points) { // increases an actors hits points by 100 (will cap at 100)
	int x = getStudentWorld()->getSocrates()->getHitPoints();
	if (x + points < 100)
		m_hitPoints += points;
	else
		m_hitPoints = 100;
}

int Actor::returnMax(int a, int b) // returns the bigger integer between a and b
{
	if (a > b)
		return a;
	else
		return b;
}

double Actor::distanceFromSocrates(int x, int y) // calculates the distance from socrates to an actor with coordinates x and y
{
	double xx = abs(getStudentWorld()->getSocrates()->getX() - x);
	double yy = abs(getStudentWorld()->getSocrates()->getY() - y);
	double distance = sqrt(pow(xx, 2) + pow(yy, 2));
	return distance;
}

bool Actor::overlapsWithSocrates(double x, double y) { // returns true if an actor with coordinates x and y overlap with socrates false otherwise

	
	double distance = distanceFromSocrates(x, y);
	if (distance <= SPRITE_RADIUS*2)
	{
		return true;
	}
	else
		return false;
}

int Actor:: distanceFromCenter(int x, int y, Direction d, int pixels) { //calculates the ditance from the center after moving pixels pixles in the directin d of the grid to an actor with original coordinates x and y 
	double xx = x;
	double yy = y;
	getPositionInThisDirection(d, pixels, xx, yy);
	double xd = abs(xx - (VIEW_WIDTH / 2)); // getting differences in x and y coordinates
	double yd = abs(yy - (VIEW_HEIGHT / 2));
	double distanceFromCenter = sqrt(pow(xd, 2) + pow(yd, 2)); // returning the length of the 'c' side of a trinagle using the pythagorean theorem
	return distanceFromCenter;
}

double Actor::angleTowardsSocrates(int x, int y) { // returns the angle between an actor with coordinates x and y and socrates
	float PI = 3.14159;
	double dirToSocratesX = getStudentWorld()->getSocrates()->getX() - x; // getting differences in x and y coordinates
	double dirToSocratesY = getStudentWorld()->getSocrates()->getY() - y;
	double angle = atan2(dirToSocratesY, dirToSocratesX); // using arctan2 so can get the radians between the two, aournd the whole grid, not just half of it
	angle = angle * (180 / PI); // converting radians to degrees

	return angle;
}

bool Actor::canBeEaten() {
	return false;
}

/////////////SOCRATES IMPLEMENTATIONS/////////////
Socrates::Socrates(StudentWorld* myStudentWorld) : Actor(IID_PLAYER, true, 0, 128, 0, 0, myStudentWorld, 100)
{
	m_sprayCharges = 20;
	m_flameThrowerCharges = 5;
	m_angle = 5;
}

void Socrates::updateFlameThrower(int num) { // adds num flame thrower charges to the number of current flame thrower charfes
	m_flameThrowerCharges += num;
}

int Socrates:: getFlameThrower() // returns the number of current flame thrower charges
{ 
	return m_flameThrowerCharges; 
}

int Socrates::getSprayCharges() // returns the current number of spray charges
{ 
	return m_sprayCharges; 
}
void Socrates::setSprayCharges(int num) //increases the number of current spray charges by num;
{
	m_sprayCharges += num;
}

bool Socrates::canBlock() { //returns false because socrates can't block objects from moving onto it
	return false;
}

bool Socrates::canBeDamaged() { //returns true because socrates can be damaged
	return true;
}

void Socrates::movement(Direction d) { // calculates one movement of socrares based on direction d 
	double PI = 4 * atan(1);
	double x = VIEW_WIDTH / 2 + VIEW_RADIUS * (cos((d + 180) * 1.0 * PI / 180));
	double y = VIEW_WIDTH / 2 + VIEW_RADIUS * (sin((d + 180) * 1.0 * PI / 180));
	moveTo(x, y);
	setDirection(d); // update direction so socrates always faces the currect direction
}

void Socrates::doSomething()
{
	if (getAlive() == false)
		return;
	if (getHitPoints() <= 0) //makes sure socrates is alive
	{
		setAlive(false);
		return;
	}
	int ch;
	Direction d = getDirection();
	Flame* f;
	Spray* sp;
	
	if (getStudentWorld()->getKey(ch)) { //gets the button pressed by user
		switch (ch)
		{
		case KEY_PRESS_LEFT: //if left arrow
			d += 5;
			movement (d);
			// move counterclockwise 5 degrees
			
			break;
		case KEY_PRESS_RIGHT: //if right arrow
			d -= 5;
			movement(d);
			
			//move clockwise 5 degrees
			break;
		case KEY_PRESS_SPACE: //if space bar
			if (m_sprayCharges >= 1) //makes sure socrates has more than 0 sprays left
			{
			sp = new Spray(getX(), getY(), d, getStudentWorld()); //allocates a new spray and adds it to the actor vector
			getStudentWorld()->addActor(sp);
			sp->moveAngle(d, 2 * SPRITE_RADIUS); // moves it forward proper amount and updates current num of spray charges
			setSprayCharges(-1);
			getStudentWorld()->playSound(SOUND_PLAYER_SPRAY);

			}
			else
				return;
			break;
		case KEY_PRESS_ENTER: //if enter bytton
			if (m_flameThrowerCharges >= 1)
			{
				//allocating 16 spray objects and added them to the actor vector, each having 22 degrees between them
			for (int i = 0; i < 16; i++)
			{
				f = new Flame(getX(), getY(), d, getStudentWorld());
				getStudentWorld()->addActor(f);
				f->moveAngle(d, 2 * SPRITE_RADIUS);
				d += 22;
			}
			updateFlameThrower(-1); //decreased th amount of flame throwes left
			getStudentWorld()->playSound(SOUND_PLAYER_FIRE);
			}
			else
					return;
			break;
		default:
			break;
		}
	}
	else
	{
		if (getSprayCharges() < 20) //if nothing is pressed, add one spray charge until totol is 20
			setSprayCharges(1);
	}

}


/////////////DIRT IMPLEMENTATIONS/////////////

Dirt::Dirt(int dirtX, int dirtY, StudentWorld* myStudentWorld) : Actor(IID_DIRT, true, dirtX, dirtY, 0, 1, myStudentWorld, 0)
{

}

bool Dirt::canBlock() { //dirt can block, so return true
	return true;
}
bool Dirt::canBeDamaged() { //dirt can be damaged, so return true
	return true;
}

/////////////PROJECTILE IMPLEMENTATIONS/////////////

Projectile::Projectile(int mid, bool alive, int proX, int proY, Direction dir, int depth, StudentWorld* myStudentWorld, int maxTravel, int decAmount) : Actor(mid, alive, proX, proY, dir, depth, myStudentWorld, 0)
{
	m_movement = 0;
	m_maxTravel = maxTravel;
	m_decAmount = decAmount;
}

void Projectile::doSomething() {
	if (getAlive() == false) //if projectile is dead, do nothing
		return;
	else if (getStudentWorld()->overlapsWithCanBeDamaged(getX(), getY(), m_decAmount))//it overlaps with a dameageable object
	{
		setAlive(false); // set it to dead and then reutrn
		return;
	}
	else
	{
		moveAngle(getDirection(), SPRITE_RADIUS * 2); //move the projectile forward proper amount
		m_movement += SPRITE_RADIUS * 2;
		if (m_movement == m_maxTravel) //once it has reached the maximum distance it has traveld, set it to dead
			setAlive(false);
	}
}

bool Projectile::canBlock() { //can't block, so return false
	return false;
}

bool Projectile::canBeDamaged() { //can't be damaged, so return false
	return false;
}

/////////////FLAME IMPLEMENTATIONS/////////////

Flame::Flame(int flameX, int flameY, Direction dir, StudentWorld* myStudentWorld) : Projectile(IID_FLAME, true, flameX, flameY, dir, 1, myStudentWorld, 32, 5)
{

}


/////////////SPRAY IMPLEMENTATIONS/////////////

Spray::Spray(int sprayX, int sprayY, Direction dir, StudentWorld* myStudentWorld) : Projectile(IID_SPRAY, true, sprayX, sprayY, dir, 1, myStudentWorld, 112, 2)
{

}

/////////////FOOD IMPLEMENTATIONS/////////////

Food::Food(int foodX, int foodY, StudentWorld* myStudentWorld) : Actor(IID_FOOD, true, foodX, foodY, 90, 1, myStudentWorld, 0) {

}

bool Food::canBlock() { //can't block, so return false
	return false;
}

bool Food::canBeDamaged() { //cant be damaged, so return false
	return false;
}

bool Food::canBeEaten() {
	return true;
}

/////////////GOODIE IMPLEMENTATIONS/////////////

Goodie::Goodie(int mid, bool alive, int gooX, int gooY, Direction dir, int depth, StudentWorld* myStudentWorld, int hitPoints, int updatePoints) : Actor(mid, alive, gooX, gooY, dir, depth, myStudentWorld, hitPoints)
{
	m_lifetime = maxx();
	m_updatePoints = updatePoints;
	m_ticks = 0;
}

void Goodie::doSomething() {
	if (getAlive() == false) //if its dead, do nothing
		return;
	else if(overlapsWithSocrates(getX(), getY()) == true) //overlaps with Socrates
	{
		getStudentWorld()->increaseScore(m_updatePoints); // add m_updatePoints to the users score
		setAlive(false); // set the goodie to dead
		getStudentWorld()->playSound(SOUND_GOT_GOODIE);
		doDifferent(); // do the specialized version of each goodie
		return;
	}

	if (m_ticks > m_lifetime) { // if goodie has been there for m_lifetime ticks wihthout socrates getting it set to dead
		setAlive(false);
	}
	m_ticks++;
	
}

bool Goodie::canBlock() { //can't block, so return false
	return false;
}

bool Goodie::canBeDamaged() { //can be damaged, so return true
	return true;
}

/////////////RESTORE HEALTH IMPLEMENTATIONS/////////////

RestoreHealth::RestoreHealth(int rhX, int rhY, StudentWorld* myStudentWorld) : Goodie(IID_RESTORE_HEALTH_GOODIE, true, rhX, rhY, 0, 1, myStudentWorld, 0, 250)
{

}

void RestoreHealth::doDifferent() {

	getStudentWorld()->getSocrates()->updateHitPoints(100); // this specifc verison of the genreal in goodie tells socrates to go to full health
}

/////////////EXTRA LIFE IMPLEMENTATIONS/////////////

ExtraLife::ExtraLife(int elX, int elY, StudentWorld* myStudentWorld) : Goodie(IID_EXTRA_LIFE_GOODIE, true, elX, elY, 0, 1,  myStudentWorld, 0, 500)
{

}

void ExtraLife::doDifferent() {
	
	getStudentWorld()->incLives(); // this specifc verison of the genreal in goodie tells socrates to increase his lives by one
}

/////////////FLAME THROWER IMPLEMENTATIONS/////////////

FlameThrower::FlameThrower(int flX, int flY, StudentWorld* myStudentWorld) : Goodie(IID_FLAME_THROWER_GOODIE, true, flX, flY, 0, 1, myStudentWorld, 0, 300)
{

}
void FlameThrower::doDifferent() {

	getStudentWorld()->getSocrates()->updateFlameThrower(5); // this specifc verison of the genreal in goodie tells socrates to add 5 to his flame thrower count
}

/////////////BACTERIA IMPLEMENTATIONS/////////////

Bacteria::Bacteria(int mid, bool alive, int bacX, int bacY, Direction dir, int depth, StudentWorld* myStudentWorld, int hitPoints, int movementPlanDistance, int damagePoints) : Actor(mid, alive, bacX, bacY, dir, depth, myStudentWorld, hitPoints)
{
	m_movementPlanDistance = movementPlanDistance;
	m_foodEaten = 0;
	m_damagePoints = damagePoints;
}

void Bacteria:: doSomething() {
	if (getAlive() == false) { //step 1 --> if dead, do nothing
		return;
	}
	bool b = true;

	aSalmonellaDifferent(b);

	if (overlapsWithSocrates(getX(), getY()) == true) //overlaps with socrates --> step 2
	{
		getStudentWorld()->getSocrates()->updateHitPoints(m_damagePoints); // make socrats hit points go down by m_damagePoints
		if (getStudentWorld()->getSocrates()->getHitPoints() > 0)
			getStudentWorld()->playSound(SOUND_PLAYER_HURT);
		else {
			getStudentWorld()->getSocrates()->setAlive(false);
			getStudentWorld()->playSound(SOUND_PLAYER_DIE);
		}
	}
	else if (m_foodEaten == 3) // step 3 --> if have eaten 3 food since its been born/last divided
	{//create new bacteria of the same kind at the same location and add it to the actor vector
		int newX = getX();
		if (newX > VIEW_WIDTH / 2)
			newX -= SPRITE_RADIUS;
		else
			newX += SPRITE_RADIUS;
		int newY = getY();
		if (newY > VIEW_WIDTH / 2)
			newY -= SPRITE_RADIUS;
		else
			newY += SPRITE_RADIUS;
		addNewActor(newX, newY);
		getStudentWorld()->setNumOfBacteria(); //update the number of bacteria
		m_foodEaten = 0;
	}
	else if (getStudentWorld()->overlapsWithFood(getX(), getY())) //overlaps with 1+ food objects --> step 4
	{
		m_foodEaten++;
		//set one food object state to dead
	}
	if (b == true)
		doDifferent1(m_movementPlanDistance); // do the specialized version of each bacteria
	else
		return;
	
}

bool Bacteria::canBlock() {
	return false;
}

bool Bacteria::canBeDamaged() {
	return true;
}


Bacteria::~Bacteria() 
{ 
	getStudentWorld()->setNumOfDeadBacteria(); // increase the number of dead bacteria by one
}

/////////////SALMONELLA IMPLEMENTATIONS/////////////
Salmonella ::Salmonella(int mid, int salX, int salY, StudentWorld* myStudentWorld, int hitPoints, int damagePoints) : Bacteria(mid, true, salX, salY, 90, 0, myStudentWorld, hitPoints, 0, damagePoints)
{

}

void Salmonella :: doDifferent1(int& movementPlanDistance) //this is the specialized version from bacteria
{
	double xx = getX();
	double yy = getY();
	getPositionInThisDirection(getDirection(), 3, xx, yy);
	if (movementPlanDistance > 0) //step 5  --> //if movement plan distance is greater than 0
	{
		movementPlanDistance--; 
		if (getStudentWorld()->overlapsWithDirt(xx, yy) || distanceFromCenter(getX(), getY(), getDirection(), 3) >= VIEW_RADIUS) //can't move forward 3 pixels its currently facing
		{
			setDirection(randInt(0, 359)); //set to a random direction
			movementPlanDistance = 10;
		}
		else {
			moveTo(xx, yy); //move 3 pixels forward in the current direction
		}
		return;
	}
	else //step 6
	{
		double xx = getX();
		double yy = getY();
		int ang = 500;
		int a = getStudentWorld()->closeToFood(xx, yy, ang);
		if (a == 500) // no food found within 128 pixels
		{
			setDirection(randInt(0, 359));
			movementPlanDistance = 10;
			return;
		}
		else
		{
			getPositionInThisDirection(ang, 3, xx, yy);  // --> change getDirection to direction of closest food
			if (getStudentWorld()->overlapsWithDirt(xx,yy)) //move 3 pixels toward food overlaps with dirt
			{
				setDirection(randInt(0, 359));
				movementPlanDistance = 10;
				return;
			}
			else
			{
				setDirection(ang);
				moveAngle(a,3);
				// move 3 pixels in direction of food
			}
		}
	}
}

/////////////REGULAR SALMONELLA IMPLEMENTATIONS/////////////

RSalmonella::RSalmonella(int rSalX, int rSalY, StudentWorld* myStudentWorld) : Salmonella(IID_SALMONELLA, rSalX, rSalY, myStudentWorld, 4, -1)
{

}

void RSalmonella::addNewActor(int x, int y) {
	RSalmonella* rSalmonella = new RSalmonella(x, y, getStudentWorld());  //specialized version of adding an actor
	getStudentWorld()->addActor(rSalmonella); 
	getStudentWorld()->playSound(SOUND_BACTERIUM_BORN);
}

/////////////AGGRESSIVE SALMONELLA IMPLEMENTATIONS/////////////

ASalmonella::ASalmonella(int aSalX, int aSalY, StudentWorld* myStudentWorld) : Salmonella(IID_SALMONELLA, aSalX, aSalY, myStudentWorld, 10, -2)
{

}

void ASalmonella::aSalmonellaDifferent(bool& a) { //this is specialized to aggressive salmonella
	if (distanceFromSocrates(getX(), getY())<= 72) // <= 72 pixels away from socrates
	{
		//get the new x and y coordinates if the agressive salmonella would move 3 pixels toward socrates
		double xx = getX();
		double yy = getY();
		int angle = angleTowardsSocrates(xx, yy);
		getPositionInThisDirection(angle, 2, xx, yy);
		if (getStudentWorld()->overlapsWithDirt(xx, yy)) //if it will get blocked by dirt
		{
			setDirection(angle);
			//will not move
		}
		else
		{
			setDirection(angle);
			moveAngle(angle, 2); //otherwise move forward 3 pixels towards socrates
		}
		a = false; //won't do something in bacteria doSomething()
	}
}

void ASalmonella::addNewActor(int x, int y) { //adds a aggressive salmonella when called
	ASalmonella* aSalmonella = new ASalmonella(x, y, getStudentWorld());
	getStudentWorld()->addActor(aSalmonella); 
	getStudentWorld()->playSound(SOUND_BACTERIUM_BORN);
}

/////////////E COLI IMPLEMENTATIONS/////////////

EColi::EColi(int eColiX, int eColiY, StudentWorld* myStudentWorld) : Bacteria(IID_ECOLI, true, eColiX, eColiY, 90, 0, myStudentWorld, 5, 0, -4)
{

}

void EColi::doDifferent1(int & movementPlanDistance)// this is the specialized version from bacteria
{
	movementPlanDistance = movementPlanDistance;
	if (distanceFromSocrates(getX(), getY()) <= 256) //less than or equal to 256 pixels away from socrates
	{
		//get directional angle theta toward socrates
		Direction theta = angleTowardsSocrates(getX(), getY()); 
		for (int i = 0; i < 10; i++)
		{
			double xx = getX();
			double yy = getY();
			getPositionInThisDirection(theta, 2, xx, yy);
			if (getStudentWorld()->overlapsWithDirt(xx, yy))//ecoli blocked by dirt moving 2 pixels toward socrates
			{
				if (theta += 10 >= 360) //wraps around at 360
				{
					theta -= 360; // --> do I need to add 10 to this??
					theta += 10;//add 10 to the angle and face that new angle
					setDirection(theta);
				}
				else {
					theta += 10; //add 10 to the angle and face that new angle
					setDirection(theta);
				}
			}
			else
			{
				moveAngle(theta, 2); //not blocked so move forward 2 pixels toward socrates
				return;
			}
		}
		return;
	}
}

void EColi::addNewActor(int x, int y) { //adds an ecoli when called
	EColi* eColi = new EColi(x, y, getStudentWorld()); 
	getStudentWorld()->addActor(eColi); 
	getStudentWorld()->playSound(SOUND_BACTERIUM_BORN);
}

/////////////FUNGUS IMPLEMENTATIONS/////////////

Fungus::Fungus(int fungusX, int fungusY, StudentWorld* myStudentWorld) : Actor(IID_FUNGUS, true, fungusX, fungusY, 0, 1, myStudentWorld, 0)
{
	m_ticks = 0;
	m_lifetime = maxx();
}

void Fungus::doSomething() {
	if (getAlive() == false) //if dead, do nothing
		return;
	if (overlapsWithSocrates(getX(), getY())==true) //overlaps with socrates
	{
		getStudentWorld()->increaseScore(-50); //decrease users score by 50 and set dead
		setAlive(false);
		getStudentWorld()->getSocrates()->updateHitPoints(-20);  // make socrates loose 20 hit points
		if (getStudentWorld()->getSocrates()->getHitPoints() > 0)
			getStudentWorld()->playSound(SOUND_PLAYER_HURT);
		else {
			getStudentWorld()->getSocrates()->setAlive(false);
			getStudentWorld()->playSound(SOUND_PLAYER_DIE);
		}
		return;
	}
	if (m_ticks >= m_lifetime) //lifetime has expired 
		setAlive(false);
	m_ticks++;
}

bool Fungus::canBlock() { //return false becasue can't block
	return false;
}

bool Fungus::canBeDamaged() { //return true becasue can be damaged
	return true;
}

/////////////PIT IMPLEMENTATIONS/////////////

Pit::Pit(int pitX, int pitY, StudentWorld* myStudentWorld) : Actor(IID_PIT, true, pitX, pitY, 0, 1, myStudentWorld, 0) //sets starting number of bacteria
{
	m_numOfASalmonella = 3; 
	m_numOfEColi = 2;
	m_numOfRSalmonella = 5;
}
void Pit::doSomething() {
	if (m_numOfASalmonella == 0 && m_numOfEColi == 0 && m_numOfRSalmonella == 0) //if the pit is empty
	{
		getStudentWorld()->setPitEmpty(); //tell student world its empty and increase number of fead pits by one
		setAlive(false);
		getStudentWorld()->setNumOfDeadPits();
	}
	else
	{
		int randI = randInt(1, 50);
		if (randI == 40) // 1 in 50 chance of happening
		{
			bool okay = false;
			while (okay == false) {
				int x = randInt(1, 3);
				if (x == 1) {
					if (m_numOfASalmonella > 0) //adding an Agressive salmonella to the game
					{
						okay = true;
						ASalmonella* aSalmonella = new ASalmonella(getX(), getY(), getStudentWorld());
						getStudentWorld()->addActor(aSalmonella);
						m_numOfASalmonella--;
						getStudentWorld()->playSound(SOUND_BACTERIUM_BORN);
					}
				}
				if (x == 1) { //adding an ecoli to the game
					if (m_numOfEColi > 0)
					{
						okay = true;
						EColi* eColi = new EColi(getX(), getY(), getStudentWorld());
						getStudentWorld()->addActor(eColi);
						m_numOfEColi--;
						getStudentWorld()->playSound(SOUND_BACTERIUM_BORN);
					}
				}
				if (x == 2) { //adding a regular salmonella to the game
					if (m_numOfRSalmonella > 0)
					{
						okay = true;
						RSalmonella* rSalmonella = new RSalmonella(getX(), getY(), getStudentWorld());
						getStudentWorld()->addActor(rSalmonella);
						m_numOfRSalmonella--;
						getStudentWorld()->playSound(SOUND_BACTERIUM_BORN);
					}
				}
			}
		}
	}
}

bool Pit::canBlock() { //returns false because can't block
	return false;
}

bool Pit::canBeDamaged() { //returns false becuase can't be damaged
	return false;
}