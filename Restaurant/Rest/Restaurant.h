#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_
#include<iomanip>
#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Events\Event.h"
#include <fstream>
#include "Order.h"
#include"../LinkedList.h"
#include"../MaxHeap.h"
#include"../MinHeap.h"
#include"Cook.h"


// it is the maestro of the project
class Restaurant  
{	
private:
	GUI *pGUI;
	// TODO: Add More Data Members As Needed
	//
	int Timestep;
	//Data members concerned with the Read function:
	int N, G, V; //The number of Normal, Vegan, VIP and Urgent cooks.
	int BO; //The number of orders a cook must prepare before taking a break while the rest are the break duration for each type of cook
	int SN_min, SN_max, SG_min, SG_max, SV_min, SV_max;	//The speed range(min& max) of each cook type
	int	BN_min, BN_max, BG_min, BG_max, BV_min, BV_max;//The break duration range(min& max) of each cook type
	float InjProp, RstPrd; //The probability a busy cook gets injured and the rest period respectively 
	int AutoP; //The number of timesteps after which an order is automatically promoted to VIP
	int VIP_WT; //the number of ticks after which a VIP order is considered urgent
	int M; //The number of events

	int Total_Cooks;//The total number of cooks
	int Total_Orders;// The total number of orders

	//Statistics to be shown on the interface.
	int VIPServed, VeganServed, NormalServed; //Used to store the number of orders of served till a specific timestep.

	//Extra data members needed for output statistics:
	int Normal_Orders_Count;
	int VIP_Orders_Count;
	int Vegan_Orders_Count;
	int Urgent_Orders_Count;
	int Injured_Cooks_Count;
	int Normal_AutoPromoted_Orders_Count;

	//Data Structures:
	//Orders:
	Queue<Order*> VeganWaitingOrders;
	MaxHeap<Order*> VIPWaitingOrders;//Could be a PQ through a linkedlist instead, sorted by VIP Priority
	LinkedList<Order*> NormalWaitingOrders; //Those not yet assigned to a cook
	MinHeap<Order*> InserviceOrders;
	Queue<Order*> FinishedOrders; //We might not need this, or might need one of each type.
	LinkedList<Order*> InserviceSim;//Used in Phase 1 simulation function only NOT used at ALL in Phase 2 

	//Events
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file
	
	//Cooks, all sorted by Speed
	MaxHeap<Cook*> NormalAvailableCooks;
	MaxHeap<Cook*> VeganAvailableCooks;
	MaxHeap<Cook*> VIPAvailableCooks;

	//Sorted by FT
	MinHeap<Cook*> OccupiedCooks;
	MinHeap<Cook*> OnBreakCooks;
	MinHeap<Cook*> RestingCooks;

	//Used in printing the orders assigned this timestep only NOT related to the servicing logic
	Queue<Order*> AssignedThisTS;	//Used to point to the orders assigned at a given timestep to print them on the status bar

	//Used Variables
	Cook* pCook;
	Order* pOrd;

	/// Utility functions:
	void ReadInputFile(); //Reads the input file and assigns values to the restaurants data members 
	void WriteOutputFile();
	void AddtoAssignedThisTS(Order* po);	//adds an order to the demo queue
	string GetAssignedThisTS();

public:
	//Basic Functions:
	Restaurant();
	~Restaurant();
	int getAutoPromoted_Orders_Count();
	int getNormal_Orders_Count();
	int getVegan_Orders_Count();
	int getVIP_Orders_Count();
	void setAutoPromoted_Orders_Count(int Cold);
	void setNormal_Orders_Count(int N);
	void setVegan_Orders_Count(int G);
	void setVIP_Orders_Count(int V);

	// Populating & Managing Lists:
	void AddtoNormalWaitingOrders(Order* po);	//adds an order to the NormalWaitingOrders LinkedList
	void AddtoVeganWaitingOrders(Order* po);	//adds an order to VeganWaitingOrders MaxHeap
	void AddtoVIPWaitingOrders(Order* po);	//adds an order to VIPWaitingOrders queue
	void AddtoInserviceOrders(Order* po);	//adds an order to InserviceOrders MinHeap
	void AddtoFinishedOrders(Order* po);	//adds an order to FinishedOrders queue
	bool RemoveFromNormalList(int ID, Order*& pO);
	//Cook Selection:
	Cook* GetCook(ORD_TYPE X);

	//Events Execution:
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	//UI Functions:
	void FillDrawingList();

	//Mode & Simulation Functions
	void RunSimulation();
	void Simple_Simulation_For_Phase_1();//a simple simulation function that tests moving orders between lists

	//Checkers
	/// Modular Functions --- Put them all here to make it easier to find them
	void ServicedOrdersChecker();
	void FreeCooksChecker();
	void RestChecker();
	void AssignCook_OrdersUrgent_VIP();
	void AssignCook_OrdersVegan_Normal();
	void AutoPromotionChecker();
	void CheckForInjury();
};

#endif