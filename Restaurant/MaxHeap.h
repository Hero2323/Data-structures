#pragma once
#include <iostream>
#include "../Restaurant/Rest/Order.h"

//Test code of max heap to use in sorting cooks by speed with the least complexity
template <class T>
class MaxHeap
{
private:
	//Data memebers
	int Count;			 //Max number elements in the heap
	int CurrentElements; //Current Number of elements in the heap
	T* Item;			 //Array of heap items

	//Utility functions

	//Swaps array elements, just for code cleanliness
	void Swap(T& A, T& B)
	{
		T temp = A;
		A = B;
		B = temp;
	}

	//Function to get the parent of a certain node
	int Parent(int i)
	{
		return (i - 1) / 2;
	}

	//Function to get the left child of a certain node given the parent index
	int LChild(int parent)
	{
		return 2 * parent + 1;
	}

	//Function to get the right child of a certain node given the parent index
	int RChild(int parent)
	{
		return 2 * parent + 2;
	}

	int GetCurrentElementCount()
	{
		return CurrentElements;
	}


public:
	//Default constructor, allocates an array of 10 by default
	MaxHeap(int	ItemCount = 100)
	{
		Item = new T[ItemCount];
		Count = ItemCount;
		CurrentElements = 0;
		for (int i = 0; i < ItemCount; i++) //Initialize every node to INT_Min as a flag
			Item[i] = nullptr;				//If there's a real node or not
	}

	//Returns the root (Max value) of the heap.
	T GetMax()
	{
		return Item[0];
	}

	//Insertion function, inserts at the last available heap index, then reheaps.
	void Insert(T NewElement)
	{
		if (CurrentElements == Count)
		{
			std::cout << "Heap Full" << std::endl;
			return;
		}

		Item[CurrentElements++] = NewElement;

		reheapUp(CurrentElements - 1);
		//Heapify2();
	}


	//Lecture implementation
	//Needed to restore the heap after inserting at the end
	void reheapUp(int NewNodeIndex)
	{
		if (NewNodeIndex != 0)
		{
			int NewNodeParent = Parent(NewNodeIndex);
			if (Item[NewNodeIndex] > Item[NewNodeParent])
			{
				Swap(Item[NewNodeIndex], Item[NewNodeParent]);
				reheapUp(NewNodeParent);
			}
		}
	}

	//Lecture implementation
	//Needed to restore the heap after removing the root
	void reheapDown(int StartingIndex, int LastNodeIndex)
	{
		 
		if (CurrentElements == 0 && CurrentElements == 1) return;

		else if (Item[RChild(StartingIndex)])
		{
			if (RChild(StartingIndex) > CurrentElements - 1) //When the heap is down to 2 elements, just does a quick comparison.
			{
				if (Item[StartingIndex] > Item[LChild(StartingIndex)])
					Swap(Item[StartingIndex], Item[LChild(StartingIndex)]);
				return;
			}
		}

		T LKey = nullptr, RKey = nullptr;
		int LargerSubtree = -1;
		//If the left or right child are out of bounds, that means you're at the end of the array 
		//NOT TESTED FOR THE MAX HEAP

		if (Item[LChild(StartingIndex)] != nullptr) //If there is an initialized left child
			LKey = Item[LChild(StartingIndex)];
		if (Item[RChild(StartingIndex)] != nullptr) //If there is an initialized right child
			RKey = Item[RChild(StartingIndex)];

		if (LKey && RKey)
		{
			if (LKey < RKey)							//Compare and assign the larger node
				LargerSubtree = LChild(StartingIndex);
			else
				LargerSubtree = RChild(StartingIndex);
		}
		else
		{
			if (LKey && !RKey)
				LargerSubtree = LChild(StartingIndex);
			else
				LargerSubtree = RChild(StartingIndex);
		}

		if (Item[StartingIndex] < Item[LargerSubtree]) //If the root is lesser than the larger 
		{												//Child, swap them and continue on down
			Swap(Item[StartingIndex], Item[LargerSubtree]);
			reheapDown(LargerSubtree, LastNodeIndex);
		}
	}

	//Replaces the largest element by the last element in the heap and sets the value of the last 
	//element as nullptr, decrements the current element cout, then returns the largest element
	bool RemoveLargest(T& Return) //Changed as per the request of Jimmy
	{
		if (!Item[0]) return false;

		Return = Item[0];
		Item[0] = Item[CurrentElements - 1];
		Item[CurrentElements - 1] = nullptr;
		CurrentElements--;
		reheapDown(0, CurrentElements - 1);
		return true;
	}

	//Prints the heap array
	void Print()
	{
		for (int i = 0; i < CurrentElements; i++)
		{
				std::cout << Item[i] << " ";
		}
		cout << '\n';
	}

	int GetHeapCount()
	{
		return Count;
	}
	

	int GetCurrentElements()
	{
		return CurrentElements;
	}
};


//===================Order specializations ====================//
template<>
void MaxHeap<Order*>::Print()
{
	for (int i = 0; i < CurrentElements; i++)
	{
		std::cout << Item[i]->getVIPPriority() << " ";
	}
	cout << '\n';
}

//Used in VIPWaitingList only and they are sorted according to the VIP priority  
template <>
void MaxHeap<Order*>::reheapUp(int NewNodeIndex)
{
	if (NewNodeIndex != 0)
	{
		int NewNodeParent = Parent(NewNodeIndex);
		if (Item[NewNodeParent]->getUrgent() == true)
		{
			if (Item[NewNodeIndex]->getUrgent() == true && Item[NewNodeIndex]->getVIPPriority() > Item[NewNodeParent]->getVIPPriority())
			{
				Swap(Item[NewNodeIndex], Item[NewNodeParent]);
				reheapUp(NewNodeParent);
			}
		}
		else
		{
			if (Item[NewNodeIndex]->getUrgent() == true || Item[NewNodeIndex]->getVIPPriority() > Item[NewNodeParent]->getVIPPriority())
			{
				Swap(Item[NewNodeIndex], Item[NewNodeParent]);
				reheapUp(NewNodeParent);
			}
		}
	}
}

template<>
void MaxHeap<Order*>::reheapDown(int StartingIndex, int LastNodeIndex)
{
	Order* LKey = nullptr, * RKey = nullptr;
	int LargerSubtree = -1;
	//If the left or right child are out of bounds, that means you're at the end of the array 
	//NOT TESTED FOR THE MAX HEAP
	//DIDN'T MODIFY IT FOR URGENT ORDERS

	if (LChild(StartingIndex) > CurrentElements - 1 || RChild(StartingIndex) > CurrentElements - 1) return;
	if (Item[LChild(StartingIndex)] != nullptr) //If there is an initialized left child
		LKey = Item[LChild(StartingIndex)];
	if (Item[RChild(StartingIndex)] != nullptr) //If there is an initialized right child
		RKey = Item[RChild(StartingIndex)];

	if (LKey > RKey)							//Compare and assign the larger node
		LargerSubtree = LChild(StartingIndex);
	else
		LargerSubtree = RChild(StartingIndex);

	if (Item[StartingIndex]->getVIPPriority() < Item[LargerSubtree]->getVIPPriority()) //If the root is lesser than the larger 
	{												//Child, swap them and continue on down
		Swap(Item[StartingIndex], Item[LargerSubtree]);
		reheapDown(LargerSubtree, LastNodeIndex);
	}
}



//=================== Cook specializations ====================//
template<>
void MaxHeap<Cook*>::Print()
{
	for (int i = 0; i < CurrentElements; i++)
	{
		std::cout << Item[i]->getSpeed() << " ";
	}
	cout << '\n';
}

template <>
void MaxHeap<Cook*>::reheapUp(int NewNodeIndex)
{
	if (NewNodeIndex != 0)
	{
		int NewNodeParent = Parent(NewNodeIndex);
		if (Item[NewNodeIndex]->getSpeed() > Item[NewNodeParent]->getSpeed())
		{
			Swap(Item[NewNodeIndex], Item[NewNodeParent]);
			reheapUp(NewNodeParent);
		}
	}
}

template<>
void MaxHeap<Cook*>::reheapDown(int StartingIndex, int LastNodeIndex)
{

	if (CurrentElements == 0 || CurrentElements == 1) return;

	else if (Item[RChild(StartingIndex)])
	{
		if (RChild(StartingIndex) > CurrentElements - 1) //When the heap is down to 2 elements, just does a quick comparison.
		{
			if (Item[StartingIndex] > Item[LChild(StartingIndex)])
				Swap(Item[StartingIndex], Item[LChild(StartingIndex)]);
			return;
		}
	}

	Cook* LKey = nullptr, *RKey = nullptr;
	int LargerSubtree = -1;
	//If the left or right child are out of bounds, that means you're at the end of the array 
	//NOT TESTED FOR THE MAX HEAP

	if (Item[LChild(StartingIndex)] != nullptr) //If there is an initialized left child
		LKey = Item[LChild(StartingIndex)];
	if (Item[RChild(StartingIndex)] != nullptr) //If there is an initialized right child
		RKey = Item[RChild(StartingIndex)];

	if (LKey && RKey)
	{
		if (LKey->getSpeed() < RKey->getSpeed())							//Compare and assign the larger node
			LargerSubtree = LChild(StartingIndex);
		else
			LargerSubtree = RChild(StartingIndex);
	}
	else
	{
		if (LKey && !RKey)
			LargerSubtree = LChild(StartingIndex);
		else
			LargerSubtree = RChild(StartingIndex);
	}

	if (!Item[LargerSubtree]) return;

	if (Item[StartingIndex]->getSpeed() < Item[LargerSubtree]->getSpeed()) //If the root is lesser than the larger 
	{												//Child, swap them and continue on down
		Swap(Item[StartingIndex], Item[LargerSubtree]);
		reheapDown(LargerSubtree, LastNodeIndex);
	}
}



