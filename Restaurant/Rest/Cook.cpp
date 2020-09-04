#include "Cook.h"

//Base cooks shouldn't be able to cook any meals
Cook::Cook()
{
	setID(0);             //These are arbitrary initial values, they should be changed
	setType(TYPE_BASE);   //while populating the cooks lists.
	setSpeed(0);
	setBreak(0);
	setOrdersDoneForBreak(0);
	setInjured(false);

}

Cook::~Cook()
{
}

void Cook::setID(int id)
{
	ID = id;
}

int Cook::GetID() const
{
	return ID;
}

void Cook::setType(ORD_TYPE t)
{
	type = t;
}

ORD_TYPE Cook::GetType() const
{
	return type;
}

void Cook::setFT(int s)
{
	FinishTime = s;
}

int Cook::getFT()
{
	return FinishTime;
}


void Cook::setOrdersDoneForBreak(int O)
{
	OrdersDoneForBreak = O;
}

int Cook::getOrdersDoneForBreak()
{
	return OrdersDoneForBreak;
}

void Cook::setSpeed(int s)
{
	speed = s > 0 ? s : 0;
}

int Cook::getSpeed()
{
	return speed;
}

void Cook::setBreak(int b)
{
	Break_Duration = b;
}

int Cook::getBreak()
{
	return Break_Duration;
}

void Cook::setBreakOrderCount(int BO)
{
	BreakOrderCount = BO;
}

int Cook::getBreakOrderCount()
{
	return BreakOrderCount;
}

bool Cook::operator>(Cook* right)
{
	return this->getSpeed() > right->getSpeed();
}

bool Cook::operator<(Cook* right)
{
	return this->getSpeed() < right->getSpeed();
}

void Cook::setInjured(bool injured)
{
	Is_Injured = injured;
}

bool Cook::getInjured()
{
	return Is_Injured;
}

