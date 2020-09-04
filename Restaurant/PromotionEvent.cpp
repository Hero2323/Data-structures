#include "PromotionEvent.h"
#include "../Restaurant/Rest/Restaurant.h"

PromotionEvent::PromotionEvent(int eTime, int ordID, float Extra) :Event(eTime, ordID)
{
	Extra_Money = Extra;
}

void PromotionEvent::Execute(Restaurant* pRest)
{
	Order* pO = nullptr;                         //Pointer used to promote the order if found
	pRest->RemoveFromNormalList(OrderID,pO);//Function used to extract the order from the NormalWaiting order list
	pRest->setNormal_Orders_Count(pRest->getNormal_Orders_Count() - 1);
	pRest->setVIP_Orders_Count(pRest->getVIP_Orders_Count() + 1);
	if (pO)   //If the order is found
	{
		pO->setType(TYPE_VIP);                   //Sets the order type to VIP                   
		pO->setMoney(pO->getMoney() + Extra_Money);  //Adds the extra money to the totalMoney of the order
		pO->calculateVIPPriority();
		pRest->AddtoVIPWaitingOrders(pO);            //Adds the order to the VIPWaiting orders list
	}
}

void PromotionEvent::Read(ifstream& Infile)
{
	Infile >> EventTime >> OrderID >> Extra_Money;
}