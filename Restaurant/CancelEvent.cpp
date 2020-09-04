#include "CancelEvent.h"
#include "Rest/Restaurant.h"

CancelEvent::CancelEvent(int eTime, int oID ) 
	:Event(eTime, oID)
{
	
}

void CancelEvent::Execute(Restaurant* pRest)
{
	Order* pO = nullptr;                       //Pointer used to delete the order if it's found
	pRest->RemoveFromNormalList(OrderID,pO);   //This function extracts the order from the NormalWaiting orders list
	if (pO)
	{
		pRest->setNormal_Orders_Count(pRest->getNormal_Orders_Count() - 1);
		delete pO;                             //Deletes the order
	}
}

void CancelEvent::Read(ifstream& Infile)
{
	Infile >> EventTime >> OrderID;
}