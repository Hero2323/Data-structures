#pragma once
#include<iostream>
#include"Generic_DS/Node.h"
#include"../Restaurant/Rest/Order.h"
using namespace std;


template<class T>
class LinkedList
{
	Node <T>* Head;
	int counter;
	Node<T>* PtrTo(int index);


public:
	bool DeleteEvents(int ID, T& Entry);
	Node<T>*& getHead();
	LinkedList();
	void Add(T item);
	Node<T>* Search(T item);
	bool Delete(T item);
	bool GetFirst(T& item);
	void Clear();
	bool Isempty();
	void DeleteNodes(T item);
	T& operator[](int index);
	bool DeleteFirst(T& Entry);
	T* toArray(int& count);
	int GetCounter(); //Added by tarek
};

template <typename T>
T* LinkedList<T>::toArray(int& count)
{
	count = 0;

	if (!Head)
		return nullptr;
	//counting the no. of items in the Queue
	Node<T>* p = Head;
	while (p)
	{
		count++;
		p = p->getNext();
	}


	T* Arr = new T[count];
	p = Head;
	for (int i = 0; i < count; i++)
	{
		Arr[i] = p->getItem();
		p = p->getNext();
	}
	return Arr;
}

template<typename T>
inline Node<T>* LinkedList<T>::PtrTo(int index)
{
	Node<T>* iterator = Head;
	while (iterator && counter != index)
	{
		iterator = iterator->getNext();
		counter++;
	}
	return iterator;
}


//Not sure if we really need it to be a template function
template <class T>
int LinkedList<T>::GetCounter()
{
	return counter;
}



template<class T>
inline bool LinkedList<T>::DeleteEvents(int ID, T& Entry)
//The entry is for copying (promotion)
{
	if (!Head)
	{
		return false;
	}

	if (Head->getItem() == ID)
	{
		Node<T>* Temp = Head;
		Head = Head->getNext();
		delete Temp;
		counter--; //Added by Tarek
		return true;
	}

	Node<T>* iterator = Head->getNext();
	Node<T>* Previous = Head;
	while (iterator)
	{
		if ((iterator->getItem()) == ID)
		{
			Node<T>* Temp = iterator;
			iterator = iterator->getNext();
			Previous->setNext(iterator);
			delete Temp;
			counter--; //Added by Tarek
			return true;
		}
		iterator = iterator->getNext();
		Previous = Previous->getNext();
	}
	return false;

}

template<class T>
inline Node<T>*& LinkedList<T>::getHead()
{
	return Head;
}

template<typename T>
inline  LinkedList<T>::LinkedList()
{
	counter = 0;
	Head = nullptr;

}

template<typename T>
inline void LinkedList<T>::Add(T item)
{
	Node<T>* d = new Node<T>;
	d->setItem(item);
	d->setNext(nullptr);
	counter++;
	if (Head == NULL)
	{
		Head = d;
		return;
	}


	Node<T>* iterator = Head;
	while (iterator->getNext())
	{
		iterator = iterator->getNext();
	}
	iterator->setNext(d);
}

template<typename T>
inline Node<T>* LinkedList<T>::Search(T item)
{
	Node<T>* iterator = Head;

	while (iterator)
	{
		if (iterator->getItem() == T)
		{
			return iterator;
		}
		iterator = iterator->getNext();
	}
	return NULL;
}

template<typename T>
inline bool LinkedList<T>::Delete(T item)
{

	if (!Head)
	{
		return false;
	}

	if (Head->getItem() == item)
	{
		Node<T>* Temp = Head;
		Head = Head->getNext();
		delete Temp;
		counter--; //Added by tarek
		return true;
	}

	Node<T>* iterator = Head->getNext();
	Node<T>* Previous = Head;
	while (iterator)
	{
		if (iterator->getItem() == item)
		{
			Node<T>* Temp = iterator;
			iterator = iterator->getNext();
			Previous->setNext(iterator);
			delete Temp;
			counter--; //Added by Tarek
			return true;
		}
		iterator = iterator->getNext();
		Previous = Previous->getNext();
	}
	return false;

}

template<typename T>
inline bool LinkedList<T>::GetFirst(T& item)
{
	if (!Head)
	{
		return false;
	}
	item = Head->getItem();
	return true;
}

template<typename T>
inline void LinkedList<T>::DeleteNodes(T item)
{
	if (!Head)
		return;



	while (Head->getItem() == item)
	{
		Node<T>* Temp = Head;
		Head = Head->getNext();
		delete Temp;
		counter--; //Added by Tarek
	}



	Node<T>* iterator = Head->getNext();
	Node<T>* Previous = Head;
	while (iterator)
	{
		if (iterator->getItem() == item)
		{
			Node<T>* Temp = iterator;
			iterator = iterator->getNext();
			Previous->setNext(iterator);
			delete Temp;
			counter--; //Added by tarek
		}
		else
		{
			iterator = iterator->getNext();
			Previous = Previous->getNext();
		}
	}
	return;

}

template<typename T>
inline bool LinkedList<T>::Isempty()
{
	return(Head == nullptr);
}

template<typename T>
inline T& LinkedList<T>::operator[](int index)
{
	return((counter > index && index >= 0) ? PtrTo(index)->getItem() : throw  "Out of Range");
}


template<typename T>
inline void LinkedList<T>::Clear()
{

	while (Head)
	{
		Node<T>* Temp = Head;
		Head = Head->getNext();
		delete Temp;
	}
	counter = 0; //Added by Tarek
}

//Deletes the first node in the list
template<typename T>
inline bool LinkedList<T>::DeleteFirst(T& Entry)
{
	if (Head)
	{
		Node<T>* P = Head->getNext();
		Head->setNext(NULL);
		Entry = Head->getItem();
		delete Head;
		Head = P;
		counter--; //Added by tarek
		return true;

	}
	return false;
}



//Specializations:
inline bool LinkedList<Order*>::DeleteEvents(int ID, Order*& Entry)
//The entry is for copying (promotion)
{
	if (!Head)                           //If List is empty
	{
		return false;
	}

	if (Head->getItem()->GetID() == ID)   //If the Order is found in the Head node
	{
		Node<Order*>* Temp = Head;
		Head = Head->getNext();
		Entry = Temp->getItem();           //This line is for the promotion event
		counter--;
		delete Temp;                       //Delete the node containint the order
		return true;
	}

	Node<Order*>* iterator = Head->getNext();      //Iterator and Previous pointers are used to delete
	Node<Order*>* Previous = Head;                 //Without chaning the order of the list
	while (iterator)
	{          //Loop untill the order is found then return true
		if ((iterator->getItem())->GetID() == ID)
		{
			Node<Order*>* Temp = iterator;
			iterator = iterator->getNext();
			Previous->setNext(iterator);
			Entry = Temp->getItem();
			counter--;
			delete Temp;
			return true;
		}
		iterator = iterator->getNext();
		Previous = Previous->getNext();
	}
	return false;    //if the order is not found.
					 //So, it's either in-service, not a normal order or there was no order with that id
}


