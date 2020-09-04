#pragma once
#include "Events/Event.h"

class PromotionEvent 
	: public Event
{
private:

	float Extra_Money;

public:

	PromotionEvent(int eTime, int ordID, float Extra);
	virtual void Read(ifstream& Infile);
	virtual void Execute(Restaurant* pRest);
};
