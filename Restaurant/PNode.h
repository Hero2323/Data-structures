#pragma once
#include "Rest/Order.h"

template<class T>
class PNode                               //Normal node class with a priority data member for Priority Queues
{
private:
	T data;
	PNode<T>* next;
	int priority;
public:
	PNode()
	{
		next = nullptr;
		priority = 0;
	}
	PNode(T d, int p)
	{
		setdata(d);
		setpriority(p);
		next = nullptr;
	}
	void setdata(T d)
	{
		data = d;
	}
	T getdata()
	{
		return data;
	}
	void setnext(PNode<T>* ptr)
	{
		next = ptr;
	}
	PNode<T>* getnext()
	{
		return next;
	}
	void setpriority(int p)
	{
		priority = p > 0 ? p : 0;
	}
	int getpriority()
	{
		return priority;
	}

};

template<>
class PNode<Order>                        //Order data type specialization
{
private:
	Order* data;
	PNode<Order>* next;
	int priority;
public:
	PNode()
	{
		next = nullptr;
		data = nullptr;
		priority = 0;
	}
	PNode(Order* d, int p)
	{
		setdata(d);
		setpriority(p);
		next = nullptr;
	}
	void setdata(Order* d)
	{
		data = d;
	}
	Order* getdata()
	{
		return data;
	}
	void setnext(PNode<Order>* ptr)
	{
		next = ptr;
	}
	PNode<Order>* getnext()
	{
		return next;
	}
	void setpriority(int p)
	{
		priority = p > 0 ? p : 0;
	}
	int getpriority()
	{
		return priority;
	}
};