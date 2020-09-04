#ifndef __ARRIVAL_EVENT_H_
#define __ARRIVAL_EVENT_H_

#include "Event.h"

//class for the arrival event
class ArrivalEvent: public Event
{
	//info about the order ralted to arrival event
	//int OrdDistance;	//order distance       //Probably a mistake
	ORD_TYPE OrdType;		//order type: Normal, vegan, VIP	                
	double OrdMoney;	//Total order money
	int OrdSize;		//Number of dishes of the order

public:
	ArrivalEvent(int eTime, int oID, ORD_TYPE oType/*,double check,int dishes*/);
	//Add more constructors if needed
	
	virtual void Execute(Restaurant *pRest);	//override execute function
	virtual void Read(ifstream& Infile);
};

#endif