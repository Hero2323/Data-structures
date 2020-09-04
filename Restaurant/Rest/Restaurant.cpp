#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>
#include "Restaurant.h"
#include "..\PromotionEvent.h"
#include "..\Events\ArrivalEvent.h"
#include "..\\CancelEvent.h"
#include "../PromotionEvent.h"
#include "../CancelEvent.h"
#include"..//Generic_DS/Node.h"
#include "Order.h"
using namespace std;

////================================================== I/O Utility Functions ==================================================
void Restaurant::ReadInputFile()
{
	//Make any necessary declarations and get the input file name
	Cook* pCook;
	string FileName;
	pGUI->PrintMessage("Enter I/P filename: ");

	FileName = pGUI->GetString();
	ifstream OpenFile(FileName + ".txt");
	while (!OpenFile.is_open()) //incase file reading fails
	{	//pGUI->ClearStatusBar();
		pGUI->PrintMessage("Unable to find the specified file, please try again. ");
		FileName = pGUI->GetString();
		OpenFile.open(FileName + ".txt");

	}
	//check if the file was found and opened successfully 

	OpenFile >> N >> G >> V;
	OpenFile >> SN_min >> SN_max >> SG_min >> SG_max >> SV_min >> SV_max;
	OpenFile >> BO >> BN_min >> BN_max >> BG_min >> BG_max >> BV_min >> BV_max;
	OpenFile >> InjProp >> RstPrd;
	OpenFile >> AutoP >> VIP_WT;
	OpenFile >> M;
	cout << InjProp << " " << RstPrd << " " << AutoP << " " << VIP_WT << " " << M;
	Total_Orders = 0;
	Total_Cooks = N + G + V;

	//Add the cooks with their data to the Available_Cooks queue
	for (int i = 0; i < N; i++)
	{
		pCook = new Cook();
		pCook->setID(i + 1);
		pCook->setSpeed(rand() % (SN_max - SN_min + 1) + SN_min);
		pCook->setType(TYPE_NRM);
		pCook->setBreak(rand() % (BN_max - BN_min + 1) + BN_min);
		pCook->setBreakOrderCount(BO);
		NormalAvailableCooks.Insert(pCook);
	}

	//then vegan cooks
	for (int i = N; i < N + G; i++)
	{
		pCook = new Cook();
		pCook->setID(i + 1);
		pCook->setType(TYPE_VGAN);
		pCook->setSpeed(rand() % (SG_max - SG_min + 1) + SG_min);
		pCook->setBreak(rand() % (BG_max - BG_min + 1) + BG_min);
		pCook->setBreakOrderCount(BO);
		VeganAvailableCooks.Insert(pCook);
	}
	//then V.I.P cooks
	for (int i = N + G; i < Total_Cooks; i++)
	{
		pCook = new Cook();
		pCook->setType(TYPE_VIP);
		pCook->setID(i + 1);
		pCook->setSpeed(rand() % (SV_max - SV_min + 1) + SV_min);
		pCook->setBreak(rand() % (BV_max - BV_min + 1) + BV_min);
		pCook->setBreakOrderCount(BO);
		VIPAvailableCooks.Insert(pCook);
	}
	//Now that we've read and populated cooks, time to deal with events
	char EventType;
	Event* pEv;
	for (int i = 0; i < M; i++)
	{
		OpenFile >> EventType;
		switch (EventType)
		{
		case 'R':
			pEv = new ArrivalEvent(0, 0, TYPE_NRM);
			pEv->Read(OpenFile);
			EventsQueue.enqueue(pEv);
			Total_Orders++;
			break;
		case 'X':
			pEv = new CancelEvent(0, 0);
			pEv->Read(OpenFile);
			EventsQueue.enqueue(pEv);
			Total_Orders--;
			break;
		case 'P':
			pEv = new PromotionEvent(0, 0, 0);
			pEv->Read(OpenFile);
			EventsQueue.enqueue(pEv);
			break;
		}
	}
	OpenFile.close();
}

void Restaurant::WriteOutputFile()
{
	pGUI->PrintMessage("Please type in the name of the output file : ");
	string name = pGUI->GetString();
	ofstream Out_File(name + ".txt");
	int Finished_No = FinishedOrders.GetCounter();
	//cout << Finished_No; 
	int waitingTot = 0;
	int ServingTot = 0;
	int FinishedTot = 0;
	Order* pOrd = nullptr;
	Out_File << "   FT   ID   AT   WT   ST" << '\n';
	for (int i = 0; i < Finished_No; i++)
	{
		FinishedOrders.dequeue(pOrd);
		waitingTot += pOrd->getWT();
		ServingTot += pOrd->getST();
		Out_File << setw(5) << pOrd->getFT() << setw(5) << pOrd->GetID() << setw(5) << pOrd->getAT() << setw(5) << pOrd->getWT() << setw(5) << pOrd->getST() << '\n';
		delete pOrd;
	}
	Out_File << "Orders : " << Finished_No << " [ Norm : " << Normal_Orders_Count << ", Veg : " << Vegan_Orders_Count << ", VIP : " << VIP_Orders_Count << " ]" << '\n'; //Counted these inside arrival events, the VIP_Orders_Count includes urgent orders
	Out_File << "Cooks : " << N + G + V << " [ Norm : " << N << ", Veg : " << G << ", VIP : " << V << ", Injured : " << Injured_Cooks_Count << " ]" << '\n'; //Counted these inside arrival events, the VIP_Orders_Count includes urgent orders
	Out_File << "AVG Wait = " << waitingTot / float(Finished_No) << ", AVG Serv :" << ServingTot / float(Finished_No) << '\n';
	Out_File << "Urgent orders :" << Urgent_Orders_Count << ", Auto - promoted : " << 100.0 * Normal_AutoPromoted_Orders_Count / (Normal_Orders_Count + Normal_AutoPromoted_Orders_Count) << "%";
	Out_File.close();
}
/////========================================= Main Simulation Functions =======================================
void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE mode = pGUI->getGUIMode();
	bool ShowInterface = false;
	bool EnableUserClick = false;
	switch (mode)
	{
	case MODE_INTR:
		ShowInterface = true;
		EnableUserClick = true;
		break;
	case MODE_STEP:
		ShowInterface = true;
		break;
	case MODE_SLNT:
		ShowInterface = false;
		break;
	};
	ReadInputFile();
	//char timestep[10];
	//pGUI->PrintMessage("Interactive Mode...Click to Continue");
	//pGUI->waitForClick();
	while (!EventsQueue.isEmpty() || VIPWaitingOrders.GetCurrentElements() != 0 || !NormalWaitingOrders.Isempty() || !VeganWaitingOrders.isEmpty() != 0
		|| InserviceOrders.GetCurrentElements() != 0)
	{
		Timestep++;
		cout << "\nTimestep: " << Timestep << '\n';

		//Cook minheap example, sorted by FT
		cout << "Occupied cooks: "; OccupiedCooks.Print(); //For debugging purposes

		//Cook maxheap example, sorted by Speed
		cout << "Normal available cooks: ";  NormalAvailableCooks.Print();

		//Order maxheap example, sorted by VIP priority
		cout << "VIP Waiting orders: "; VIPWaitingOrders.Print();

		//Order minheap example, sorted by FT
		cout << "Inservice orders: "; InserviceOrders.Print();

		cout << "\n\n";

		ExecuteEvents(Timestep);
		ServicedOrdersChecker();
		FreeCooksChecker();
		RestChecker();
		CheckForInjury();  //This could either be placed here(before assigning free cooks their orders) or after assigning all cooks
		AutoPromotionChecker();
		AssignCook_OrdersUrgent_VIP();
		AssignCook_OrdersVegan_Normal();
		if (ShowInterface)
		{
			FillDrawingList();
			pGUI->UpdateInterface();
			string msg = "TS: " + to_string(Timestep) + "\n Waiting Orders: N:" + to_string(NormalWaitingOrders.GetCounter());
			msg += " G: " + to_string(VeganWaitingOrders.GetCounter()) + " V: " + to_string(VIPWaitingOrders.GetCurrentElements());
			msg += "\n Available Cooks: N: " + to_string(NormalAvailableCooks.GetCurrentElements()) + " G: " + to_string(VeganAvailableCooks.GetCurrentElements());
			msg += " V:" + to_string(VIPAvailableCooks.GetCurrentElements()) + "\n Assigned Last TS: ";
			msg += GetAssignedThisTS();
			msg += "\n Orders Served: N: " + to_string(NormalServed) + " G: " + to_string(VeganServed) + " V: " + to_string(VIPServed);
			pGUI->PrintMessage(msg);
			if (EnableUserClick)
				pGUI->waitForClick();
			else
				Sleep(1000);
			pGUI->ResetDrawingList();
		}
	}
	WriteOutputFile();
}

void Restaurant::Simple_Simulation_For_Phase_1()
{
	//=====Perform any needed intializations
	Order* pOrd;

	//=====Call file loading funciton
	ReadInputFile();
	pGUI->PrintMessage("Testing data structures and moving orders between lists ...CLICK to continue");
	pGUI->waitForClick();

	//=====Execute Events Each timestep=====
	int CurrentTimeStep = 1;
	//as long as events queue is not empty yet
	while (!EventsQueue.isEmpty() || VIPWaitingOrders.GetMax() != NULL || !NormalWaitingOrders.Isempty() || !VeganWaitingOrders.isEmpty() || !InserviceSim.Isempty())
	{
		//print current timestep
		char timestep[10];
		itoa(CurrentTimeStep, timestep, 10);
		pGUI->PrintMessage(timestep);


		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step

		//Move one order of each type to InserviceSim each timestep
		if (VIPWaitingOrders.RemoveLargest(pOrd)) //Got to add way too many conditions regarding cooks.
		{
			pOrd->setStatus(SRV);
			InserviceSim.Add(pOrd);
			//has to be a for loop (ends when orders are done/cooks or done)
		}
		if (NormalWaitingOrders.DeleteFirst(pOrd))
		{
			pOrd->setStatus(SRV);
			InserviceSim.Add(pOrd);

		}
		if (VeganWaitingOrders.dequeue(pOrd))
		{
			pOrd->setStatus(SRV);
			InserviceSim.Add(pOrd);
			//SRVG Time=Ratio

		}

		if (CurrentTimeStep % 5 == 0)

		{
			Node<Order*>* temp = InserviceSim.getHead();
			while (temp)
			{
				if (temp->getItem()->GetType() == TYPE_NRM)
				{
					temp->getItem()->setStatus(DONE);
					FinishedOrders.enqueue(temp->getItem());
					InserviceSim.Delete(temp->getItem());
					break;

					//Deallocate finished orders>>>>? ((no needs to store)
					//Time condition

				}
				temp = temp->getNext();
			}
			temp = InserviceSim.getHead();
			while (temp)
			{
				if (temp->getItem()->GetType() == TYPE_VIP)
				{
					temp->getItem()->setStatus(DONE);
					FinishedOrders.enqueue(temp->getItem());
					InserviceSim.Delete(temp->getItem());
					break;
				}
				temp = temp->getNext();
			}
			temp = InserviceSim.getHead();

			while (temp)
			{
				if (temp->getItem()->GetType() == TYPE_VGAN)
				{
					temp->getItem()->setStatus(DONE);
					FinishedOrders.enqueue(temp->getItem());
					InserviceSim.Delete(temp->getItem());
					break;
				}
				temp = temp->getNext();
			}


		}
		//Adding orders/cooks to the drawing list
		FillDrawingList();
		pGUI->UpdateInterface();
		pGUI->PrintWaitingAndAvailble(VIPWaitingOrders.GetCurrentElements(), NormalWaitingOrders.GetCounter(), VeganWaitingOrders.GetCounter(), VIPAvailableCooks.GetCurrentElements(), NormalAvailableCooks.GetCurrentElements(), VeganAvailableCooks.GetCurrentElements());
		pGUI->waitForClick();
		//Sleep(1000);
		CurrentTimeStep++;	//advance timestep
		pGUI->ResetDrawingList();
	}

	pGUI->PrintMessage("Simple Simulation successfully done, click to END program");
	pGUI->waitForClick();
}
/////====================================================== Events Excution functions ===========================================================
void Restaurant::ExecuteEvents(int CurrentTimeStep)//Executes ALL events that should take place at current timestep
{
	Event* pE;
	while (EventsQueue.peekFront(pE))	//as long as there are more events
	{
		if (pE->getEventTime() > CurrentTimeStep)	//no more events at current timestep
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}
/////====================================================== Modular Functions ===========================================================
void Restaurant::ServicedOrdersChecker()
{
	MinHeap<Order*> SortedByST;
	while (InserviceOrders.GetCurrentElements() != 0 && InserviceOrders.GetMin()->getFT() == Timestep)
	{
		InserviceOrders.RemoveSmallest(pOrd);
		pOrd->setStatus(DONE);
		if (pOrd->GetType() == TYPE_NRM)
			NormalServed++;
		else if (pOrd->GetType() == TYPE_VGAN)
			VeganServed++;
		else if (pOrd->GetType() == TYPE_VIP)
			VIPServed++;
		pOrd->setFT(pOrd->getST());
		SortedByST.Insert(pOrd);
		//For debugging purposes
		/*cout << "Timestep: " << Timestep << " Outgoing order:" << pOrd->GetID() << " " << pOrd->getFT();
		cout << '\n';
		InserviceOrders.Print();
		cout << '\n';*/
	}
	while (SortedByST.RemoveSmallest(pOrd))
	{
		pOrd->setFT(Timestep);
		FinishedOrders.enqueue(pOrd);
	}

}

void Restaurant::FreeCooksChecker()
{
	//So this function first checks if the cook needs a break, or if he's injured, if not it reinserts the cook to their 
	//Proper heap.
	//Doesn't seem problematic..

	//If there are any occupied cooks and the first occupied cook should be finish on this TS
	while (OccupiedCooks.GetCurrentElements() != 0 && OccupiedCooks.GetMin()->getFT() == Timestep)
	{
		OccupiedCooks.RemoveSmallest(pCook);
		pCook->setOrdersDoneForBreak(pCook->getOrdersDoneForBreak() + 1);
		//If the cook has enough orders for a break
		if (pCook->getInjured())
		{
			pCook->setOrdersDoneForBreak(0);

			pCook->setFT(Timestep + RstPrd);
			RestingCooks.Insert(pCook);
		}
		else if (pCook->getOrdersDoneForBreak() >= pCook->getBreakOrderCount())
		{
			//If the cook is injured and needs a break, he takes a rest first and is moved to the resting cooks heap.

			//If uninjured, the number of orders done for a break is reset and the cook takes a break in the occupied cooks list.

			pCook->setOrdersDoneForBreak(0);
			pCook->setFT(Timestep + pCook->getBreak());
			OnBreakCooks.Insert(pCook);

		}
		else if (pCook->getInjured())
		{
			pCook->setFT(Timestep + RstPrd);
			RestingCooks.Insert(pCook);
		}
		//If the cook doesn't need a break or a rest, they are moved to their proper heap.
		else if (pCook->GetType() == TYPE_VIP)
			VIPAvailableCooks.Insert(pCook);
		else if (pCook->GetType() == TYPE_NRM)
			NormalAvailableCooks.Insert(pCook);
		else
			VeganAvailableCooks.Insert(pCook);
	}
}

void Restaurant::RestChecker()
{
	//Checks if there are any resting cooks (Cooks on a break or resting cooks)
	//If the top of the heap exists and they're supposed to get back to work on this TS
	//Checks if they're injured sets them to 'uninjured'
	//Then resets the number of orders done before needing a break whether the cook was resting or on-break
	//Lastly, it reinserts the cook into their proper heap.
	while (OnBreakCooks.GetCurrentElements() != 0 && OnBreakCooks.GetMin()->getFT() == Timestep)
	{
		OnBreakCooks.RemoveSmallest(pCook);
		if (pCook->GetType() == TYPE_VIP)
			VIPAvailableCooks.Insert(pCook);
		else if (pCook->GetType() == TYPE_NRM)
			NormalAvailableCooks.Insert(pCook);
		else
			VeganAvailableCooks.Insert(pCook);
	}

	while (RestingCooks.GetCurrentElements() != 0 && RestingCooks.GetMin()->getFT() == Timestep)
	{
		RestingCooks.RemoveSmallest(pCook);
		pCook->setInjured(false);
		pCook->setOrdersDoneForBreak(0);
		if (pCook->GetType() == TYPE_VIP)
			VIPAvailableCooks.Insert(pCook);
		else if (pCook->GetType() == TYPE_NRM)
			NormalAvailableCooks.Insert(pCook);
		else
			VeganAvailableCooks.Insert(pCook);
	}
}

void Restaurant::AssignCook_OrdersUrgent_VIP()
{
	int ST;
	while ((VIPWaitingOrders.GetCurrentElements() != 0 && (VIPAvailableCooks.GetCurrentElements() != 0 ||
		NormalAvailableCooks.GetCurrentElements() != 0 || VeganAvailableCooks.GetCurrentElements() != 0))
		|| (VIPWaitingOrders.GetCurrentElements() != 0 && VIPWaitingOrders.GetMax()->getUrgent() == true))
	{
		VIPWaitingOrders.RemoveLargest(pOrd);
		bool UsedRestingCooks = false;
		if (pOrd->getUrgent())
		{
			if (VIPAvailableCooks.GetCurrentElements() != 0)
				VIPAvailableCooks.RemoveLargest(pCook);
			else if (NormalAvailableCooks.GetCurrentElements() != 0)
				NormalAvailableCooks.RemoveLargest(pCook);
			else if (VeganAvailableCooks.GetCurrentElements() != 0)
				VeganAvailableCooks.RemoveLargest(pCook);
			else if (OnBreakCooks.GetCurrentElements() != 0)
			{
				OnBreakCooks.GetMin()->setOrdersDoneForBreak(OnBreakCooks.GetMin()->getBreakOrderCount());
				OnBreakCooks.RemoveSmallest(pCook);
			}
			else if (RestingCooks.GetCurrentElements() != 0)
			{
				UsedRestingCooks = true;
				RestingCooks.RemoveSmallest(pCook);
			}
			else
			{
				VIPWaitingOrders.Insert(pOrd);
				break;
			}
			if (UsedRestingCooks)
				ST = ceil(pOrd->getOrdSize() / (double(pCook->getSpeed()) / 2));
			else
				ST = ceil(pOrd->getOrdSize() / double(pCook->getSpeed()));
		}
		else
		{
			if (VIPAvailableCooks.GetCurrentElements() != 0)
				VIPAvailableCooks.RemoveLargest(pCook);
			else if (NormalAvailableCooks.GetCurrentElements() != 0)
				NormalAvailableCooks.RemoveLargest(pCook);
			else if (VeganAvailableCooks.GetCurrentElements() != 0)
				VeganAvailableCooks.RemoveLargest(pCook);
			ST = ceil(pOrd->getOrdSize() / double(pCook->getSpeed()));
		}
		pOrd->setST(ST);
		pOrd->setWT(Timestep - pOrd->getAT());
		pOrd->setFT(pOrd->getAT() + pOrd->getWT() + pOrd->getST());
		pCook->setFT(pOrd->getAT() + pOrd->getWT() + pOrd->getST());
		InserviceOrders.Insert(pOrd);
		pOrd->setStatus(SRV);
		pOrd->setOrderCook(pCook);
		OccupiedCooks.Insert(pCook);
		AddtoAssignedThisTS(pOrd);

		//For debugging purposes
		if (OccupiedCooks.GetMin()->getFT() < Timestep)
		{
			cout << "BUG";
		}
	}
}

void Restaurant::AssignCook_OrdersVegan_Normal()
{
	int ST;
	//Service the Vegan orders with available vegan cooks
	while (!VeganWaitingOrders.isEmpty() && VeganAvailableCooks.GetCurrentElements() != 0)
	{
		VeganWaitingOrders.dequeue(pOrd);
		VeganAvailableCooks.RemoveLargest(pCook);
		ST = ceil(pOrd->getOrdSize() / double(pCook->getSpeed()));
		pOrd->setST(ST);
		pOrd->setWT(Timestep - pOrd->getAT());
		pOrd->setFT(pOrd->getAT() + pOrd->getWT() + pOrd->getST());
		pCook->setFT(pOrd->getAT() + pOrd->getWT() + pOrd->getST());
		InserviceOrders.Insert(pOrd);
		pOrd->setStatus(SRV);
		pOrd->setOrderCook(pCook);
		OccupiedCooks.Insert(pCook);
		AddtoAssignedThisTS(pOrd);

		//For debugging purposes
		if (OccupiedCooks.GetMin()->getFT() < Timestep)
		{
			cout << "BUG";
		}
	}
	//Then normal orders if there are normal cooks available
	//Changed so it assigns normal orders to normal cooks then VIP cooks -Tarek
	while (!NormalWaitingOrders.Isempty() && (NormalAvailableCooks.GetCurrentElements() != 0 || VIPAvailableCooks.GetCurrentElements() != 0))
	{
		//Added after checking the doc -Tarek
		if (NormalAvailableCooks.GetCurrentElements() != 0)
			NormalAvailableCooks.RemoveLargest(pCook);
		else if (VIPAvailableCooks.GetCurrentElements() != 0)
			VIPAvailableCooks.RemoveLargest(pCook);

		NormalWaitingOrders.DeleteFirst(pOrd);
		ST = ceil(pOrd->getOrdSize() / double(pCook->getSpeed()));
		pOrd->setST(ST);
		pOrd->setWT(Timestep - pOrd->getAT());
		pOrd->setFT(pOrd->getAT() + pOrd->getWT() + pOrd->getST());
		pCook->setFT(pOrd->getAT() + pOrd->getWT() + pOrd->getST());
		InserviceOrders.Insert(pOrd);
		pOrd->setStatus(SRV);
		pOrd->setOrderCook(pCook);
		OccupiedCooks.Insert(pCook);
		AddtoAssignedThisTS(pOrd);

		//For debugging purposes
		if (OccupiedCooks.GetMin()->getFT() < Timestep)
		{
			cout << "BUG";
		}

	}
}

void Restaurant::AutoPromotionChecker()
{
	Order* pOrder;
	Queue<Order*> Temp;//Needed to traverse the VIPWaitingOrders max heap
	//Check if a VIP order became an Urgent one
	while (VIPWaitingOrders.RemoveLargest(pOrder))
	{
		if (Timestep - pOrder->getAT() > VIP_WT && !pOrder->getUrgent())
		{
			pOrder->setUrgent(true);
			Urgent_Orders_Count++;
			VIP_Orders_Count;
		}

		Temp.enqueue(pOrder);
	}
	while (Temp.dequeue(pOrder))
		AddtoVIPWaitingOrders(pOrder);//VIP orders are sorted according to whether they are urgent orders or not then by the Priority equation

	pOrder = NULL;
	//Check if a normal order became a VIP one
	while (NormalWaitingOrders.GetFirst(pOrder) && (Timestep - pOrder->getAT() > AutoP))
	{
		RemoveFromNormalList(pOrder->GetID(), pOrder);
		pOrder->setType(TYPE_VIP);
		pOrder->calculateVIPPriority();
		setNormal_Orders_Count(getNormal_Orders_Count() - 1);
		setVIP_Orders_Count(getVIP_Orders_Count() + 1);
		setAutoPromoted_Orders_Count(getAutoPromoted_Orders_Count() + 1);
		AddtoVIPWaitingOrders(pOrder);
	}

}

void Restaurant::CheckForInjury()
{

	//This should check the first busy cook (OccupiedCooks.peek(pCook)) and generate a random number between 0 and 32767 (RAND_MAX), gets number % 100 and converts
	//That into a percentages
	//If the percentage <= InjProp and there's an occupied cook
	//Makes the first occupied cook injured (if not already injured) and halves their speed, also changes the order FT 
	//Then reinserts the cook and order back into their respective data structure.


	//Nice job Jimmy!


	if (OccupiedCooks.GetCurrentElements() != 0)
	{
		//Moved the next 2 lines inside so they only calculate when needed.
		float Inj = rand() % 100;
		Inj = Inj / 100.0; //We need a float percentage.....

		if (Inj <= InjProp)
		{
			pCook = OccupiedCooks.GetMin();     //Checks for the first busy cook. i.e. the one who's going to finish the soonest
			if (pCook->getInjured() == false)   //If he's not injured
			{
				OccupiedCooks.RemoveSmallest(pCook);      //Remove the cook from the occupied list as his FT is going to change
				InserviceOrders.RemoveSmallest(pOrd);     //Same for the order
				pCook->setInjured(true);   //Sets him as injured
				Injured_Cooks_Count++; //increment injured count
				int TimeSpentOnOrder = Timestep - (pOrd->getAT() + pOrd->getWT());
				//as the cook speed is halved, the amount of time left for the injury before is doubled
				//that amount is the old service time minus the time already spent on the order
				//So, the total service time of the order is the (Time left for the order * 2) + the TimeSpentOnOrder 
				pOrd->setST((pOrd->getST() - TimeSpentOnOrder) * 2 + TimeSpentOnOrder);
				pOrd->setFT(pOrd->getAT() + pOrd->getWT() + pOrd->getST());
				pCook->setFT(pOrd->getFT());
				InserviceOrders.Insert(pOrd);  //Reinsert the order into the list so that its new position in the list can be calculated
				OccupiedCooks.Insert(pCook);   //same as the order
			}
		}
	}
}
/////=================================================== Interface Related Utility Functions ====================================================
string Restaurant::GetAssignedThisTS()
{
	Order* pOrder;
	string AssignedList;
	while (AssignedThisTS.dequeue(pOrder))
	{
		switch (pOrder->getOrderCook()->GetType())
		{
		case TYPE_NRM:
			AssignedList += "N";
			break;
		case TYPE_VGAN:
			AssignedList += "G";
			break;
		case TYPE_VIP:
			AssignedList += "V";
			break;
		default:
			break;
		}
		AssignedList += to_string(pOrder->getOrderCook()->GetID());
		AssignedList += "(";
		switch (pOrder->GetType())
		{
		case TYPE_NRM:
			AssignedList += "N";
			break;
		case TYPE_VGAN:
			AssignedList += "G";
			break;
		case TYPE_VIP:
			AssignedList += "V";
			break;
		default:
			break;
		}
		AssignedList += to_string(pOrder->GetID());
		AssignedList += ") ";
	}
	return AssignedList;
}

void Restaurant::AddtoAssignedThisTS(Order* po)
{
	AssignedThisTS.enqueue(po);
}
/////========================================= Main Functions (Setters, Getters, Constructor and Destructor) =============================================
Restaurant::Restaurant()
{
	Normal_Orders_Count = 0;
	VIP_Orders_Count = 0;
	Vegan_Orders_Count = 0; //No. of orders of each type
	Urgent_Orders_Count = 0;
	Injured_Cooks_Count = 0;
	Normal_AutoPromoted_Orders_Count = 0; //no. of promoted normal orders
	pGUI = NULL;
	NormalServed = 0;
	VeganServed = 0;
	VIPServed = 0;
	Timestep = 0;
	N = 0;
	G = 0;
	V = 0;
	BO = 0;
	SN_min = 0;
	SN_max = 0;
	SG_min = 0;
	SG_max = 0;
	SV_min = 0;
	SV_max = 0;
	BN_min = 0;
	BN_max = 0;
	BG_min = 0;
	BG_max = 0;
	BV_min = 0;
	BV_max = 0;
	InjProp = 0;
	RstPrd = 0;
	AutoP = 0;
	VIP_WT = 0;
	M = 0;

}

Restaurant::~Restaurant()
{
	if (pGUI)
		delete pGUI;
}

int Restaurant::getAutoPromoted_Orders_Count()
{
	return Normal_AutoPromoted_Orders_Count;
}

int Restaurant::getNormal_Orders_Count()
{
	return Normal_Orders_Count;
}

int Restaurant::getVIP_Orders_Count()
{
	return VIP_Orders_Count;
}

int Restaurant::getVegan_Orders_Count()
{
	return Vegan_Orders_Count;
}

void Restaurant::setAutoPromoted_Orders_Count(int Cold)
{
	Normal_AutoPromoted_Orders_Count = Cold;
}

void Restaurant::setNormal_Orders_Count(int N)
{
	Normal_Orders_Count = N;
}

void Restaurant::setVegan_Orders_Count(int G)
{
	Vegan_Orders_Count = G;
}

void Restaurant::setVIP_Orders_Count(int V)
{
	VIP_Orders_Count = V;
}
/////============================================  Populating & Managing Lists Functions =======================================================
void Restaurant::AddtoNormalWaitingOrders(Order* po)
{
	NormalWaitingOrders.Add(po);

}

void Restaurant::AddtoVeganWaitingOrders(Order* po)
{
	VeganWaitingOrders.enqueue(po);

}

void Restaurant::AddtoVIPWaitingOrders(Order* po)
{
	VIPWaitingOrders.Insert(po);

}

void Restaurant::AddtoInserviceOrders(Order* po)//Called to move orders to the in-service MinHeap
{
	InserviceOrders.Insert(po);
}

void Restaurant::AddtoFinishedOrders(Order* po)//Called to move orders to the in-service Queue
{
	FinishedOrders.enqueue(po);
}

bool Restaurant::RemoveFromNormalList(int ID, Order*& pO)     //This function Deletes the node containing the order from
{                                                             //The NormalWaiting orders list if found
	return NormalWaitingOrders.DeleteEvents(ID, pO);          //It also returns a pointer to the order
}                                                             //So that it can be deleted or promoted according to need

Cook* Restaurant::GetCook(ORD_TYPE X)//This function is for testing Phase 1. It mainly tests moving orders between lists and printing the IDs using FillDrawingList function.It calls ReadInputFile.
{
	Cook* Cp = nullptr;
	switch (X)
	{
	case TYPE_NRM:
		NormalAvailableCooks.RemoveLargest(Cp);
		break;
	case TYPE_VGAN:
		VeganAvailableCooks.RemoveLargest(Cp);
		break;
	case TYPE_VIP:
		VIPAvailableCooks.RemoveLargest(Cp);
	}
	return Cp;
}

/////============================================ UI Functions =======================================================
void Restaurant::FillDrawingList()
{

	//This function should be implemented in phase1
	//It should add ALL orders and Cooks to the drawing list
	//It should get orders from orders lists/queues/stacks/whatever (same for Cooks)
	//To add orders it should call function  void GUI::AddToDrawingList(Order* pOrd);
	//To add Cooks it should call function  void GUI::AddToDrawingList(Cook* pCc);

	//======================Declare any needed Pointers and Temporary Queues=================
	Order* pOrd;
	Cook* pC;
	Queue<Order*> TempO;
	Queue<Cook*> Temp;
	MinHeap<Cook*> TempC;
	MinHeap<Order*>TempV;

	/////////////////////////////////////////////////////////////////////////////////////////
	//==================================Available Cooks======================================
	while (NormalAvailableCooks.RemoveLargest(pC))//Check the existance of an element and remove it 
	{
		TempC.Insert(pC);//Store them in MinHeap that sorts them according to availability time
		Temp.enqueue(pC);//Store the elements temporarily in the same order as the original 
	}
	while (Temp.dequeue(pC))//Return all the elements to their original data structure
		NormalAvailableCooks.Insert(pC);


	while (VeganAvailableCooks.RemoveLargest(pC))//Check the existance of an element and remove it 
	{
		TempC.Insert(pC);//Store them in MinHeap that sorts them according to availability time
		Temp.enqueue(pC);//Store the elements temporarily in the same order as the original 
	}
	while (Temp.dequeue(pC))//Return all the elements to their original data structure
		VeganAvailableCooks.Insert(pC);


	while (VIPAvailableCooks.RemoveLargest(pC))//Check the existance of an element and remove it 
	{
		TempC.Insert(pC);//Store them in MinHeap that sorts them according to availability time
		Temp.enqueue(pC);//Store the elements temporarily in the same order as the original 
	}
	while (Temp.dequeue(pC))//Return all the elements to their original data structure
		VIPAvailableCooks.Insert(pC);

	while (TempC.RemoveSmallest(pC))//Check the existance of an element and remove it 
	{
		pGUI->AddToDrawingList(pC);//Add the element to the DrawingList
	}

	//==================================Waiting Orders=======================================
	while (VIPWaitingOrders.RemoveLargest(pOrd))//Check the existance of an element and remove it 
	{
		pOrd->setFT(pOrd->getAT());
		TempV.Insert(pOrd);//Store them in MinHeap that sorts them according to Arrival time
	}
	while (TempV.RemoveSmallest(pOrd))
	{
		pGUI->AddToDrawingList(pOrd);//Add the element to the DrawingList
		pOrd->setFT(0);
		VIPWaitingOrders.Insert(pOrd);
	}
	while (VeganWaitingOrders.dequeue(pOrd))//Check the existance of an element and remove it 
	{
		pGUI->AddToDrawingList(pOrd);//Add the element to the DrawingList
		TempO.enqueue(pOrd);//Store the elements temporarily in the same order as the original 
	}
	while (TempO.dequeue(pOrd))//Return all the elements to their original data structure
		VeganWaitingOrders.enqueue(pOrd);
	while (NormalWaitingOrders.DeleteFirst(pOrd))//Check the existance of an element and remove it 
	{
		pGUI->AddToDrawingList(pOrd);//Add the element to the DrawingList
		TempO.enqueue(pOrd);//Store the elements temporarily in the same order as the original 
	}
	while (TempO.dequeue(pOrd))//Return all the elements to their original data structure
		NormalWaitingOrders.Add(pOrd);

	//=================================In-Service Orders====================================
	while (InserviceOrders.RemoveSmallest(pOrd))//Check the existance of an element and remove it 
	{
		pGUI->AddToDrawingList(pOrd);//Add the element to the DrawingList
		TempO.enqueue(pOrd);//Store the elements temporarily in the same order as the original 
	}
	while (TempO.dequeue(pOrd))//Return all the elements to their original data structure
		InserviceOrders.Insert(pOrd);
	//Added for the linkedlist
	int size = 0; //should be added as a data member for phase II
	Order** SO = InserviceSim.toArray(size);
	for (int i = 0; i < size; i++)
		pGUI->AddToDrawingList(SO[i]);

	//================================Serviced Orders=======================================
	while (FinishedOrders.dequeue(pOrd))//Check the existance of an element and remove it 
	{
		pGUI->AddToDrawingList(pOrd);//Add the element to the DrawingList
		TempO.enqueue(pOrd);//Store the elements temporarily in the same order as the original 
	}
	while (TempO.dequeue(pOrd))//Return all the elements to their original data structure
		FinishedOrders.enqueue(pOrd);
}