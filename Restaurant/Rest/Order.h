#pragma once
#ifndef __ORDER_H_
#define __ORDER_H_
#include<cmath>
#include "..\Defs.h"

class Cook;

class Order
{

protected:
	
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status;	//waiting, in-service, done
	                
	double totalMoney;	//Total order money

	int ArrTime,WaitTime , ServTime, FinishTime;	//arrival, service start, and finish times
	
	int OrdSize; //No. of dishes**
	bool Urgent;
	//Used for VIP Orders only to sort them
	double VIPPriority;
	Cook* OrderCook;
	
public:
	Order();
	Order(int ID, ORD_TYPE r_Type, int OrderSize, double OrderPrice, int Timestamp);
	virtual ~Order();

	int GetID();

	ORD_TYPE GetType() const;

	bool operator > (Order* right);

	bool operator < (Order* right);

	//Fresh
	bool operator == (int id); 
	void setStatus(ORD_STATUS s);
	ORD_STATUS getStatus() const;
	void setType(ORD_TYPE T);
	ORD_TYPE getType();
	void setMoney(int M);
	int getMoney();
	int getOrdSize();
	void setWT(int w);
	int getWT();
	void setST(int s);
	void setFT(int f);
	int getST();
	int getFT();
	int getAT();
	double getVIPPriority();
	//Function containing the Priority Equation that calculates the priority for each VIP order 
	void calculateVIPPriority();
	void setOrderCook(Cook* C);
	Cook* getOrderCook();
	void setUrgent(bool);
	bool getUrgent();
};

#endif