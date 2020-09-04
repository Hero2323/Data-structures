#include "Order.h"
#include <algorithm>
using namespace std;


Order::Order(int id, ORD_TYPE r_Type, int OrderSize, double OrderPrice, int Timestamp)
{
	ID = (id > 0 && id < 1000) ? id : 0;	//1<ID<999
	type = r_Type;
	status = WAIT;
	OrdSize = OrderSize;
	totalMoney = OrderPrice;
	ArrTime = Timestamp;
	Urgent = false;
	OrderCook = nullptr;
	WaitTime = 0;
	ServTime = 0;
	FinishTime = 0;
	VIPPriority = 0;
}

Order::Order()
{
	
}

Order::~Order()
{

}

int Order::GetID()
{
	return ID;
}


ORD_TYPE Order::GetType() const
{
	return type;
}

bool Order::operator>(Order* right)
{
	return (this->getFT() > right->getFT());
}

bool Order::operator<(Order* right)
{
	return (this->getFT() < right->getFT());
}




void Order::setStatus(ORD_STATUS s)
{
	status = s;
}

ORD_STATUS Order::getStatus() const
{
	return status;
}

void Order::setType(ORD_TYPE T)
{
	type = T;
}

ORD_TYPE Order::getType()
{
	return type;
}

void Order::setMoney(int M)
{
	totalMoney = M > 0 ? M : 0;
}

int Order::getMoney()
{
	return totalMoney;
}

int Order::getOrdSize()
{
	return OrdSize;
}

void Order::setWT(int w)
{
	WaitTime = w;
}

int Order::getWT()
{
	return WaitTime;
}

void Order::setST(int s)
{
	ServTime = s;
}

void Order::setFT(int FT)
{
	FinishTime = FT;
}

int Order::getST()
{
	return ServTime;
}

int Order::getFT()
{
	return FinishTime;
}

int Order::getAT()
{
	return ArrTime;
}

void Order::setOrderCook(Cook* C)
{
	OrderCook = C;
}

Cook* Order::getOrderCook()
{
	return OrderCook;
}

bool Order ::operator==(int id)
{
	return (id == ID);

}

void Order::setUrgent(bool Urg)
{
	Urgent = Urg;
}

bool Order::getUrgent()
{
	return Urgent;
}

double Order::getVIPPriority()
{
	return VIPPriority;
}

//Function containing the Priority Equation that calculates the priority for each VIP order 
void Order::calculateVIPPriority()
{
	if (type == TYPE_VIP)//Additional Check to make sure that any other order type doesn't have a priority except according to the finish time
	{
		VIPPriority = (abs(OrdSize - 25.0) * OrdSize + pow(2, ArrTime)) * totalMoney / 400;
	}
	else
		VIPPriority = 0;
}