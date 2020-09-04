#pragma once

#pragma once
#include "PNode.h"

template <class T>                           //Priority	Queue data structure which is similar to queue but all elemements are sorted
class PQueue                                 //in an order of their priority
{
private:
	PNode<T>* Head;
public:
	PQueue()
	{
		Head = nullptr;
	}
	bool Push(T data, int p)
	{
		PNode<T>* New = new PNode<T>(data, p);
		if (!Head)     //If queue is emtpy
		{
			Head = New;       //Creates a new node and asigns the head pointer to it
			return true;
		}
		if (Head->getpriority() < New->getpriority())           //if the quese isnt' emtpy, it checks the priority of the head
		{
			New->setnext(Head);
			Head = Head->getnext();
			Head = New;
			return true;
		}
		PNode<T>* ptr = Head;
		while (ptr->getnext())                                   //if the priority of the head is higher than the new item
		{
			if (ptr->getnext()->getpriority() < New->getpriority())       //checks the following items 
			{
				New->setnext(ptr->getnext());
				ptr->setnext(New);
				return true;
			}
			ptr = ptr->getnext();
		}
		ptr->setnext(New);
		return true;
	}
	PNode<T>* Pop()
	{
		if (Head)                  //if node isn't emtpy
		{
			PNode<T>* Temp = Head;
			Head = Head->getnext();
			Temp->setnext(nullptr);          //setting the next of the popped node with null to seperate it from the list
			return Temp;
		}
		return nullptr;
	}
	void IncrementPriority()                //Increments all the element's priorities with 1
	{                                       //This function should be used at the beginning of each time step
		PNode<T>* ptr = Head;               //So that orders that have been waiting for more time have higher priority
		while (ptr)
		{
			ptr->setpriority(ptr->getpriority() + 1);
			ptr = ptr->getnext();
		}
	}
};
   


//Temporary working algorithm (to be discussed)
template<>
class PQueue<Order>          //Queue specialization for class Order
{
private:
	PNode<Order>* Head;
public:
	PQueue()
	{
		Head = nullptr;
	}
	bool Push(Order* data, int p)
	{
		PNode<Order>* New = new PNode<Order>(data, p);
		if (!Head)
		{
			Head = New;
			return true;
		}
		if (Head->getpriority() < New->getpriority())
		{
			New->setnext(Head);
			Head = Head->getnext();
			Head = New;
			return true;
		}
		PNode<Order>* ptr = Head;
		while (ptr->getnext())
		{
			if (ptr->getnext()->getpriority() < New->getpriority())
			{
				New->setnext(ptr->getnext());
				ptr->setnext(New);
				return true;
			}
			ptr = ptr->getnext();
		}
		ptr->setnext(New);
		return true;
	}
	PNode<Order>* Pop()
	{
		if (Head)
		{
			PNode<Order>* Temp = Head;
			Head = Head->getnext();
			Temp->setnext(nullptr);
			return Temp;
		}
		return nullptr;
	}
	void IncrementPriority()
	{
		PNode<Order>* ptr = Head;
		while (ptr)
		{
			ptr->setpriority(ptr->getpriority() + 1);
			ptr = ptr->getnext();
		}
	}
	PNode<Order>* PopFirstNormal()                     //If we decide to put all the order types in one list
	{                                                  //This function can be used to quickly extract a normal type order
		if (!Head) { return nullptr; }
		if (Head->getdata()->GetType() == TYPE_NRM)
		{
			PNode<Order>* Temp = Head;
			Head = Head->getnext();
			Temp->setnext(nullptr);
			return Temp;
		}
		PNode<Order>* ptr = Head;
		while (ptr->getnext())
		{
			if (ptr->getnext()->getdata()->GetType() == TYPE_NRM)
			{
				PNode<Order>* Temp = ptr->getnext();
				ptr->setnext(Temp->getnext());
				Temp->setnext(nullptr);
				return Temp;
			}
			ptr = ptr->getnext();
		}
		return nullptr;
	}
	PNode<Order>* PopFirstVegan()                                          //If we decide to put all the order types in one list
	{                                                                      //This function can be used to quickly extract a vegan type order
		if (!Head) { return nullptr; }
		if (Head->getdata()->GetType() == TYPE_VGAN)
		{
			PNode<Order>* Temp = Head;
			Head = Head->getnext();
			Temp->setnext(nullptr);
			return Temp;
		}
		PNode<Order>* ptr = Head;
		while (ptr->getnext())
		{
			if (ptr->getnext()->getdata()->GetType() == TYPE_VGAN)
			{
				PNode<Order>* Temp = ptr->getnext();
				ptr->setnext(Temp->getnext());
				Temp->setnext(nullptr);
				return Temp;
			}
			ptr = ptr->getnext();
		}
		return nullptr;
	}
};