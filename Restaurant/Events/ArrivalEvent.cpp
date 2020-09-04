#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"


ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType) :Event(eTime, oID)
{
	OrdType = oType;
}

void ArrivalEvent::Execute(Restaurant* pRest)
{
	//This function should create an order and fills its data 
	// Then adds it to normal, vegan, or VIP order lists that you will create in phase1


	///For the sake of demo, this function will just create an order and add it to DemoQueue
	///Remove the next code lines in phases 1&2

	Order* pOrd = new Order(OrderID, OrdType,OrdSize,OrdMoney,EventTime);
	pOrd->calculateVIPPriority();
	switch (OrdType)
	{
	case TYPE_NRM:
		pRest->AddtoNormalWaitingOrders(pOrd);
		pRest->setNormal_Orders_Count(pRest->getNormal_Orders_Count() + 1);
		break;
	case TYPE_VGAN:
		pRest->AddtoVeganWaitingOrders(pOrd);
		pRest->setVegan_Orders_Count(pRest->getVegan_Orders_Count() + 1);
		break;
	case TYPE_VIP:
		pRest->AddtoVIPWaitingOrders(pOrd);
		pRest->setVIP_Orders_Count(pRest->getVIP_Orders_Count() + 1);
		break;
	}
	
}

void ArrivalEvent::Read(ifstream& Infile)
{
	char TYP;
	Infile >> TYP;
	switch (TYP)
	{
	case 'N':
		OrdType = TYPE_NRM;
		break;
	case 'G':
		OrdType = TYPE_VGAN;
		break;
	case 'V':
		OrdType = TYPE_VIP;
		break;
	}
	Infile >> EventTime >> OrderID >> OrdSize >> OrdMoney;
}
