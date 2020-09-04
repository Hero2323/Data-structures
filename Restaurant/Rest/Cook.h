#pragma once
#include "..\Defs.h"
#include <fstream>
#include "Order.h"
using namespace std;

#pragma once
class Cook
{
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		//dishes it can prepare in one clock tick (in one timestep)
	int Break_Duration; //For the bonus
	int BreakOrderCount;
	int OrdersDoneForBreak;
	int FinishTime;
	bool Is_Injured;

public:
	Cook();
	virtual ~Cook();

	void setID(int);
	int GetID() const;

	void setType(ORD_TYPE) ;
	ORD_TYPE GetType() const;

	void setFT(int s);
	int getFT();

	void setOrdersDoneForBreak(int O);
	int getOrdersDoneForBreak();

	//Remember to change these if we add more
	//Order types     

	void setSpeed(int s);
	int getSpeed();

	void setBreak(int);
	int getBreak();

	void setBreakOrderCount(int BO);
	int getBreakOrderCount();

	
	bool operator > (Cook* right);

	bool operator < (Cook* right);
	
	void setInjured(bool);
	bool getInjured();


};
