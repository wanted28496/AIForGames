#include "ofApp.h"
#include "KinematicSeek.h"
#include "KinematicArrive.h"
#include "Structures.h"
#include "Boid.h"
#include <Windows.h>
#include <Psapi.h>
#include "AStar.h"
#include "SLCGraph.h"
#include "LargeGraphCreator.h"
#include "TileGraph.h"

#include "DynamicPathFollow.h"
#include "DynamicAlign.h"

#if _WIN32
#define MAX UINT32_MAX
#elif _WIN64
#define MAX UINT64_MAX
#endif

#pragma region Memory Checking FUnctions

DWORDLONG GetPhysicalMemory() {
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
	DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
	return physMemUsed;
}

DWORDLONG GetVirtualMemory() {
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
	DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
	return virtualMemUsed;
}

void PrintMemoryConsumption(DWORDLONG iVirtual, DWORDLONG iPhy) {
	cout << "Virtual Memory Usage: " << iVirtual / 1048576 << " MB" << endl;
	cout << "Physical memory used: " << iPhy / 1048576 << " MB" << endl;
}

#pragma endregion



void RunSmallGraph() {
	
	vector<int> smallGraphMap =
	{
		///		1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3
		/*1*/   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*2*/   0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*3*/   0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*4*/   0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*5*/   0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*6*/   0, 2, 3, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*7*/   0, 0, 0, 3, 0, 2, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*8*/   0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*9*/   0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*10*/  0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*11*/  0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 0, 1, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*12*/  0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 3, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*13*/  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*14*/  10, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*15*/  0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*16*/  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*17*/  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*18*/  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*19*/  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*20*/  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 8, 0,
		/*21*/  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 0, 0, 0, 0, 1, 0, 1, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*22*/  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1, 0, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/*23*/  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 1, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0,
		/*24*/  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 4, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0,
		/*25*/  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2, 0, 3, 0, 0, 2, 0, 0, 0, 0,
		/*26*/  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0,
		/*27*/  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 3, 0,
		/*28*/  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 5, 0,
		/*29*/  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 0, 2, 0, 0, 4,
		/*30*/  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0,
		/*31*/  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0,
		/*32*/  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 3, 5, 0, 0, 4, 0, 0,
		/*33*/  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0
		///		1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3
	};

	SLCGraph smallGraph(33, smallGraphMap);
	int x, y;
	do
	{
		DWORDLONG startVirtual = GetVirtualMemory();
		DWORDLONG startPhysical = GetPhysicalMemory();
		DWORDLONG endVirtual;
		DWORDLONG endPhysical;
		cout << endl;
		cout << "-------------------------------------------------" << endl;
		cout << "Path Finding on SLC Map Graph...." << endl;
		cout << "Enter Source and Destination in range from 0 to 32" << endl;
		cout << "Enter -1 for source and destination to exit" << endl;
		cin >> x >> y;

		if (x <= -1 || x >= 33 || y <= -1 || y >= 33)
			continue;

		/// Manhattan Distance
		{
			smallGraph.UseManhattanDistance();
			auto path = AStar::findPath(x, y, smallGraph.getGraph());
			cout << "A* path using Random Hueristic is: ";
			for (auto node : path)
			{
				cout << node << " -> ";
			}
			cout << endl;
			endVirtual = GetVirtualMemory();
			endPhysical = GetPhysicalMemory();
			PrintMemoryConsumption((endVirtual > startVirtual) ? endVirtual - startVirtual : startVirtual - endVirtual, (endPhysical > startPhysical) ? endPhysical - startPhysical : startPhysical - endPhysical);
			cout << endl;
		}

		/// Constant Hueistic
		{
			// Change this value for change in constant Hueristic
			int constant = 6;
			smallGraph.UseConstantDistance(constant);
			auto path = AStar::findPath(x, y, smallGraph.getGraph());
			cout << "A* path using constant Distance is: ";
			for (auto node : path)
			{
				cout << node << " -> ";
			}
			cout << endl;
			endVirtual = GetVirtualMemory();
			endPhysical = GetPhysicalMemory();
			PrintMemoryConsumption((endVirtual > startVirtual) ? endVirtual - startVirtual : startVirtual - endVirtual, (endPhysical > startPhysical) ? endPhysical - startPhysical : startPhysical - endPhysical);
			cout << endl;
		}

		/// Overestimation of Hueristic
		{
			smallGraph.UseOverEstimatedHueristic();
			auto path = AStar::findPath(x, y, smallGraph.getGraph());
			cout << "A* path using Over Estimated Hueristic is: ";
			for (auto node : path)
			{
				cout << node << " -> ";
			}
			cout << endl;
			endVirtual = GetVirtualMemory();
			endPhysical = GetPhysicalMemory();
			PrintMemoryConsumption((endVirtual > startVirtual) ? endVirtual - startVirtual : startVirtual - endVirtual, (endPhysical > startPhysical) ? endPhysical - startPhysical : startPhysical - endPhysical);
			cout << endl;
		}

		/// Dijstra Algorithm
		{
			smallGraph.UseDijkstraAlgorith();
			auto path = AStar::findPath(x, y, smallGraph.getGraph());
			cout << "Dijkstra path is: ";
			for (auto node : path)
			{
				cout << node << " -> ";
			}
			cout << endl;
			endVirtual = GetVirtualMemory();
			endPhysical = GetPhysicalMemory();
			PrintMemoryConsumption((endVirtual > startVirtual) ? endVirtual - startVirtual : startVirtual - endVirtual, (endPhysical > startPhysical) ? endPhysical - startPhysical : startPhysical - endPhysical);
			cout << endl;
		}
	} while(x != -1 && y != -1);
}

void RunLargeGraph() {
	DWORDLONG startVirtual = GetVirtualMemory();
	DWORDLONG startPhysical = GetPhysicalMemory();
	DWORDLONG endVirtual;
	DWORDLONG endPhysical;
	cout << endl;
	cout << "-------------------------------------------------" << endl;
	int count, x , y;
	do
	{
		cout << "Enter number of Nodes to create Graph, minimum 1000 max 10000 : ";
		cin >> count;
	} while (count < 1000 || count > 10000);
	
	LargeGraphCreator largeGraph(count);
	do
	{
		cout << "Path Finding on Large auto generated Graph...." << endl;
		cout << "Enter Source and Destination in range from 0 to " << count - 1 << endl;
		cout << "Enter -1 for source and destination to exit" << endl;
		cin >> x >> y;

		if (x <= -1 || x >= count || y <= -1 || y >= count)
			continue;

		/// Manhattan Distance
		{
			largeGraph.UseManhattanDistance();
			auto path = AStar::findPath(x, y, largeGraph.getGraph());
			cout << "Total Cost: " << largeGraph.getGraph().GetCost(path) << endl;
			cout << "A* path using Manhattan Distance is: ";
			for (auto node : path)
			{
				cout << node << " -> ";
			}
			cout << endl;
			endVirtual = GetVirtualMemory();
			endPhysical = GetPhysicalMemory();
			//PrintMemoryConsumption((endVirtual > startVirtual) ? endVirtual - startVirtual : startVirtual - endVirtual, (endPhysical > startPhysical) ? endPhysical - startPhysical : startPhysical - endPhysical);
			cout << endl;
		}

		/// Constant Hueistic
		{
			// Change this value for change in constant Hueristic
			int constant = 5;
			largeGraph.UseConstantDistance(constant);
			auto path = AStar::findPath(x, y, largeGraph.getGraph());
			cout << "A* path using constant Distance is: ";
			for (auto node : path)
			{
				cout << node << " -> ";
			}
			cout << endl;
			endVirtual = GetVirtualMemory();
			endPhysical = GetPhysicalMemory();
			PrintMemoryConsumption((endVirtual > startVirtual) ? endVirtual - startVirtual : startVirtual - endVirtual, (endPhysical > startPhysical) ? endPhysical - startPhysical : startPhysical - endPhysical);
			cout << endl;
			cout << endl;
		}

		 ///Eculidian Disatance of Hueristic
		{
			largeGraph.UseEuclideanHuerisitc();
			auto path = AStar::findPath(x, y, largeGraph.getGraph());
			cout << "A* path using Euclidean distance as Hueristic is: ";
			for (auto node : path)
			{
				cout << node << " -> ";
			}
			cout << endl;
			endVirtual = GetVirtualMemory();
			endPhysical = GetPhysicalMemory();
			//PrintMemoryConsumption((endVirtual > startVirtual) ? endVirtual - startVirtual : startVirtual - endVirtual, (endPhysical > startPhysical) ? endPhysical - startPhysical : startPhysical - endPhysical);
			cout << endl;
			cout << endl;
		}

		//For testing different Heuristics Uncomment the code blocks from below below

		/// Random Hueistic 0 - 100
		//{
		//	// Change this value for change in constant Hueristic
		//	largeGraph.UseRandomHueristic(0, 100);
		//	auto path = AStar::findPath(x, y, largeGraph.getGraph());
		//	cout << "Total Cost: " << largeGraph.getGraph().GetCost(path) << endl;
		//	cout << "A* path using random Distance using 0 to 100 range is : ";
		//	for (auto node : path)
		//	{
		//		cout << node << " -> ";
		//	}
		//	cout << endl;
		//	cout << endl;
		//	endVirtual = GetVirtualMemory();
		//	/*endPhysical = GetPhysicalMemory();
		//	PrintMemoryConsumption((endVirtual > startVirtual) ? endVirtual - startVirtual : startVirtual - endVirtual, (endPhysical > startPhysical) ? endPhysical - startPhysical : startPhysical - endPhysical);
		//	cout << endl;*/
		//}

		/// Random Hueistic 0 - 1000
		//{
		//	// Change this value for change in constant Hueristic
		//	largeGraph.UseRandomHueristic(0, 1000);
		//	auto path = AStar::findPath(x, y, largeGraph.getGraph());
		//	cout << "Total Cost: " << largeGraph.getGraph().GetCost(path) << endl;
		//	cout << "A* path using random Distance using 0 to 1000 range is : ";
		//	for (auto node : path)
		//	{
		//		cout << node << " -> ";
		//	}
		//	cout << endl;
		//	cout << endl;
		//	endVirtual = GetVirtualMemory();
		//	/*endPhysical = GetPhysicalMemory();
		//	PrintMemoryConsumption((endVirtual > startVirtual) ? endVirtual - startVirtual : startVirtual - endVirtual, (endPhysical > startPhysical) ? endPhysical - startPhysical : startPhysical - endPhysical);
		//	cout << endl;*/
		//}

		/// Random Hueistic 0 - number of nodes
		//{
		//	// Change this value for change in constant Hueristic
		//	largeGraph.UseRandomHueristic(0, count);
		//	auto path = AStar::findPath(x, y, largeGraph.getGraph());
		//	cout << "Total Cost: " << largeGraph.getGraph().GetCost(path) << endl;
		//	cout << "A* path using random Distance using 0 to number of nodes range is : ";
		//	for (auto node : path)
		//	{
		//		cout << node << " -> ";
		//	}
		//	cout << endl;
		//	cout << endl;
		//	endVirtual = GetVirtualMemory();
		//	/*endPhysical = GetPhysicalMemory();
		//	PrintMemoryConsumption((endVirtual > startVirtual) ? endVirtual - startVirtual : startVirtual - endVirtual, (endPhysical > startPhysical) ? endPhysical - startPhysical : startPhysical - endPhysical);
		//	cout << endl;*/
		//}
		
		/// Random Hueistic 0 - MAX
		//{
		//	// Change this value for change in constant Hueristic
		//	largeGraph.UseRandomHueristic(0, MAX);
		//	auto path = AStar::findPath(x, y, largeGraph.getGraph());
		//	cout << "Total Cost: " << largeGraph.getGraph().GetCost(path) << endl;
		//	cout << "A* path using random Distance using 0 to max range is : ";
		//	for (auto node : path)
		//	{
		//		cout << node << " -> ";
		//	}
		//	cout << endl;
		//	endVirtual = GetVirtualMemory();
		//	/*endPhysical = GetPhysicalMemory();
		//	PrintMemoryConsumption((endVirtual > startVirtual) ? endVirtual - startVirtual : startVirtual - endVirtual, (endPhysical > startPhysical) ? endPhysical - startPhysical : startPhysical - endPhysical);
		//	cout << endl;*/
		//}

		/// Random Manhattan Hueistic 0 - 1000
		///// Random Manhattan Distance
		//{
		//	largeGraph.UseRandomManhattenHueristic(0, 1000);
		//	auto path = AStar::findPath(x, y, largeGraph.getGraph());
		//	cout << "Total Cost: " << largeGraph.getGraph().GetCost(path) << endl;
		//	cout << "A* path using Random Manhattan Distance using 0 to 1000 range is: ";
		//	for (auto node : path)
		//	{
		//		cout << node << " -> ";
		//	}
		//	cout << endl;
		//	cout << endl;
		//	endVirtual = GetVirtualMemory();
		//	endPhysical = GetPhysicalMemory();
		//	/*PrintMemoryConsumption((endVirtual > startVirtual) ? endVirtual - startVirtual : startVirtual - endVirtual, (endPhysical > startPhysical) ? endPhysical - startPhysical : startPhysical - endPhysical);
		//	cout << endl;*/
		//}

		/// Random Manhattan Hueistic 0 - MAX
		///// Random Manhattan Distance
		//{
		//	largeGraph.UseRandomManhattenHueristic(0, MAX);
		//	auto path = AStar::findPath(x, y, largeGraph.getGraph());
		//	cout << "Total Cost: " << largeGraph.getGraph().GetCost(path) << endl;
		//	cout << "A* path using Random Manhattan Distance using 0 to max range is: ";
		//	for (auto node : path)
		//	{
		//		cout << node << " -> ";
		//	}
		//	cout << endl;
		//	cout << endl;
		//	endVirtual = GetVirtualMemory();
		//	endPhysical = GetPhysicalMemory();
		//	/*PrintMemoryConsumption((endVirtual > startVirtual) ? endVirtual - startVirtual : startVirtual - endVirtual, (endPhysical > startPhysical) ? endPhysical - startPhysical : startPhysical - endPhysical);
		//	cout << endl;*/
		//}

        ///Dijstra Algorithm
		{
		largeGraph.UseDijkstraAlgorith();
		auto path = AStar::findPath(x, y, largeGraph.getGraph());
		cout << "Total Cost: " << largeGraph.getGraph().GetCost(path) << endl;
		cout << "Dijkstra path is: ";
		for (auto node : path)
		{
			cout << node << " -> ";
		}
		cout << endl;
		endVirtual = GetVirtualMemory();
		cout << endl;
		endPhysical = GetPhysicalMemory();
		//PrintMemoryConsumption((endVirtual > startVirtual) ? endVirtual - startVirtual : startVirtual - endVirtual, (endPhysical > startPhysical) ? endPhysical - startPhysical : startPhysical - endPhysical);
		//cout << endl;
		}

	} while (x != -1 && y != -1);
	

}

//--------------------------------------------------------------
void ofApp::setup(){

	int index;
	do
	{
		cout << "---Menu for Pathfinding algorithms---" << endl;
		cout << "Enter 1 to start pathfinding on small graph" << endl;
		cout << "Enter 2 to start pathfinding on large graph" << endl;
		cout << "Enter 3 to start graphical demonstration of pathfinding with movement algorithms*" << endl;
		cout << "*after starting you cannot run path finding on small and large graphs untill you rerun the project" << endl;
		cin >> index;

		if (index == 1)
		{
			RunSmallGraph();
		}
		else if (index == 2)
		{
			RunLargeGraph();
		}

	} while (index != 3);
	

	mTileGraph = new TileGraph(Width, Height, 50, 50);
	mTileGraph->GenerateObstacles(100);

	mPlayer = new Boid(ofVec2f(10, 10));
	mPlayer->mSteering = [this]()
	{
		auto moveSteering = DynamicPathFollow::GetSteering(mPlayer->mKinematic, mPlayer->mTargets);
		auto rotationSteering = DynamicLookWhereYouAreGoing::GetSteering(mPlayer->mKinematic);
		return moveSteering + rotationSteering;
	};

}

//--------------------------------------------------------------
void ofApp::update(){
	mPlayer->UpdateBoid(ofGetLastFrameTime());
}

//--------------------------------------------------------------
void ofApp::draw(){
	mTileGraph->DrawTileGraph();
	mPlayer->DrawBoid();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

	auto path = AStar::findPath(mTileGraph->QauntizePosition(mPlayer->mKinematic.mPosition), mTileGraph->QauntizePosition(ofVec2f(x, y)), mTileGraph->GetGraph());

	queue<KinematicStructure> localPath;
	for (auto node : path)
	{
		KinematicStructure destination;
		destination.mPosition = mTileGraph->LocalizedPosition(node);
		localPath.push(destination);
	}

	mPlayer->SetTaregts(localPath);

	//if(currentAlgorithm == Algorithms::KinematicArrive || currentAlgorithm == Algorithms::DynamicArrive)
	//{
	//	sTarget.SetBoidLocation(ofVec2f(x, y));
	//	//kArriveAlgo.mTarget.mPosition = ofVec2f(x, y);
	//	isTimerActive = true;
	//}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
