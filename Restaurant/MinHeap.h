#pragma once
#include <iostream>



//Test code of max heap to use in sorting cooks by speed with the least complexity
template <class T>
class MinHeap
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

public:
	//Default constructor, allocates an array of 10 by default
	MinHeap(int	ItemCount = 100)
	{
		Item = new T[ItemCount];
		Count = ItemCount;
		CurrentElements = 0;
		for (int i = 0; i < ItemCount; i++) //Initialize every node to nullptr as a flag
			Item[i] = nullptr;				//If there's a real node or not
	}

	int GetCurrentElements()
	{
		return CurrentElements;
	}

	//Returns the root (Max value) of the heap.
	T GetMin()
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
			if (Item[NewNodeIndex] < Item[NewNodeParent])
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
		T LKey = 0, RKey = 0;
		int SmallerSubtree = 0;
		if (LChild(StartingIndex) > CurrentElements - 1 || RChild(StartingIndex) > CurrentElements - 1) return;
		if (Item[LChild(StartingIndex)] != nullptr) //If there is an initialized left child
			LKey = Item[LChild(StartingIndex)];
		if (Item[RChild(StartingIndex)] != nullptr) //If there is an initialized right child
			RKey = Item[RChild(StartingIndex)];

		if (LKey < RKey)							//Compare and assign the larger node
			SmallerSubtree = LChild(StartingIndex);
		else
			SmallerSubtree = RChild(StartingIndex);

		if (Item[StartingIndex] > Item[SmallerSubtree]) //If the root is lesser than the larger 
		{												//Child, swap them and continue on down
			Swap(Item[StartingIndex], Item[SmallerSubtree]);
			reheapDown(SmallerSubtree, LastNodeIndex);
		}
	}

	//Replaces the largest element by the last element in the heap and sets the value of the last 
	//element as INT_MIN, decrements the current element cout, then returns the largest element
	bool RemoveSmallest(T& Return) //Changed as per the request of Jimmy
	{
		if (!Item[0]) return false;
		Return = Item[0];
		Item[0] = Item[CurrentElements - 1];
		Item[CurrentElements - 1] = nullptr;
		CurrentElements--;
		reheapDown(0, CurrentElements);
		return true;
	}

	//Prints the heap array
	void Print()
	{
		for (int i = 0; i < CurrentElements; i++)
			std::cout << Item[i] << " ";
		std::cout << "\n";
	}

	int GetHeapCount()
	{
		return Count;
	}
};




//=================== Cook specializations ====================//

template<>
void MinHeap<Cook*>::Print()
{
	for (int i = 0; i < CurrentElements; i++)
		std::cout << Item[i]->getFT() << " ";
	std::cout << "\n";
}

template<>
void MinHeap<Cook*>::reheapUp(int NewNodeIndex)
{
	if (NewNodeIndex != 0)
	{
		int NewNodeParent = Parent(NewNodeIndex);
		if (Item[NewNodeIndex]->getFT() < Item[NewNodeParent]->getFT()) //Swap if the child is lesser than the parent.
		{
			Swap(Item[NewNodeIndex], Item[NewNodeParent]);
			reheapUp(NewNodeParent);
		}
	}
}

template<>
void MinHeap<Cook*>::reheapDown(int StartingIndex, int LastNodeIndex)
{
	
	if (CurrentElements == 0 || CurrentElements == 1 || !Item[StartingIndex]) return;

	else if (CurrentElements == 2)
	{
		if (Item[StartingIndex]->getFT() > Item[LChild(StartingIndex)]->getFT())
			Swap(Item[StartingIndex], Item[LChild(StartingIndex)]);
		return;

	}

	Cook* LKey = nullptr; Cook* RKey = nullptr;
	int SmallerSubtree = 0;


	if (Item[LChild(StartingIndex)] != nullptr) //If there is an initialized left child
		LKey = Item[LChild(StartingIndex)];
	if (Item[RChild(StartingIndex)] != nullptr) //If there is an initialized right child
		RKey = Item[RChild(StartingIndex)];

	if (LKey && RKey)
	{
		if (LKey->getFT() < RKey->getFT())							//Compare and assign the larger node
			SmallerSubtree = LChild(StartingIndex);
		else
			SmallerSubtree = RChild(StartingIndex);
	}
	else
	{
		if (LKey && !RKey)
			SmallerSubtree = LChild(StartingIndex);
		else
			SmallerSubtree = RChild(StartingIndex);
	}

	if (!Item[SmallerSubtree]) return;

	if (Item[StartingIndex]->getFT() > Item[SmallerSubtree]->getFT()) //If the root is lesser than the larger 
	{												//Child, swap them and continue on down
		Swap(Item[StartingIndex], Item[SmallerSubtree]);
		reheapDown(SmallerSubtree, LastNodeIndex);
	}
}



//===================Order specializations ====================//
template<>
void MinHeap<Order*>::Print()
{
	for (int i = 0; i < CurrentElements; i++)
		std::cout << Item[i]->getFT() << " ";
	std::cout << "\n";
}

template<>
void MinHeap<Order*>::reheapUp(int NewNodeIndex)
{
	if (NewNodeIndex != 0)
	{
		int NewNodeParent = Parent(NewNodeIndex);
		if (Item[NewNodeIndex]->getFT() < Item[NewNodeParent]->getFT())
		{
			Swap(Item[NewNodeIndex], Item[NewNodeParent]);
			reheapUp(NewNodeParent);
		}
	}
}

template<>
void MinHeap<Order*>::reheapDown(int StartingIndex, int LastNodeIndex)
{
	if (CurrentElements == 0 || CurrentElements == 1 || !Item[StartingIndex]) return;

	else if (CurrentElements == 2)
	{
		if (Item[StartingIndex]->getFT() > Item[LChild(StartingIndex)]->getFT())
			Swap(Item[StartingIndex], Item[LChild(StartingIndex)]);
		return;

	}

	Order* LKey = nullptr; Order* RKey = nullptr;
	int SmallerSubtree = 0;


	if (Item[LChild(StartingIndex)] != nullptr) //If there is an initialized left child
		LKey = Item[LChild(StartingIndex)];
	if (Item[RChild(StartingIndex)] != nullptr) //If there is an initialized right child
		RKey = Item[RChild(StartingIndex)];

	if (LKey && RKey)
	{
		if (LKey->getFT() < RKey->getFT())							//Compare and assign the larger node
			SmallerSubtree = LChild(StartingIndex);
		else
			SmallerSubtree = RChild(StartingIndex);
	}
	else
	{
		if (LKey && !RKey)
			SmallerSubtree = LChild(StartingIndex);
		else
			SmallerSubtree = RChild(StartingIndex);
	}

	if (!Item[SmallerSubtree]) return;

	if (Item[StartingIndex]->getFT() > Item[SmallerSubtree]->getFT()) //If the root is lesser than the larger 
	{												//Child, swap them and continue on down
		Swap(Item[StartingIndex], Item[SmallerSubtree]);
		reheapDown(SmallerSubtree, LastNodeIndex);
	}
}