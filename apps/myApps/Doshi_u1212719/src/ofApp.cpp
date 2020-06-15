#include "ofApp.h"
#include "Structures.h"
#include "Boid.h"
#include <Windows.h>
#include <Psapi.h>

#include "AStar.h"
#include "SmallGraph.h"
#include "LargeGraphCreator.h"
#include "TileGraph.h"

#include "DTActionBase.h"
#include "DT.h"
#include "DTManager.h"
#include "DTNode.h"

#include "DTWalk.h"
#include "DTSleep.h"
#include "DTDig.h"
#include "DTLearnedActions.h"

#include "BT.h"
#include "BTAction.h"
#include "BTBlackboard.h"
#include "BTTick.h"

#include "Goap.h"

#include "DynamicPathFollow.h"
#include "DynamicAlign.h"
#include "DynamicWander.h"

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

	SmallGraph smallGraph(33, smallGraphMap);
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
			/*PrintMemoryConsumption((endVirtual > startVirtual) ? endVirtual - startVirtual : startVirtual - endVirtual, (endPhysical > startPhysical) ? endPhysical - startPhysical : startPhysical - endPhysical);*/
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
			/*PrintMemoryConsumption((endVirtual > startVirtual) ? endVirtual - startVirtual : startVirtual - endVirtual, (endPhysical > startPhysical) ? endPhysical - startPhysical : startPhysical - endPhysical);*/
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
			/*PrintMemoryConsumption((endVirtual > startVirtual) ? endVirtual - startVirtual : startVirtual - endVirtual, (endPhysical > startPhysical) ? endPhysical - startPhysical : startPhysical - endPhysical);*/
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
			/*PrintMemoryConsumption((endVirtual > startVirtual) ? endVirtual - startVirtual : startVirtual - endVirtual, (endPhysical > startPhysical) ? endPhysical - startPhysical : startPhysical - endPhysical);*/
			cout << endl;
		}
	} while (x != -1 && y != -1);
}

void RunLargeGraph() {
	DWORDLONG startVirtual = GetVirtualMemory();
	DWORDLONG startPhysical = GetPhysicalMemory();
	DWORDLONG endVirtual;
	DWORDLONG endPhysical;
	cout << endl;
	cout << "-------------------------------------------------" << endl;
	int count, x, y;
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
			/*PrintMemoryConsumption((endVirtual > startVirtual) ? endVirtual - startVirtual : startVirtual - endVirtual, (endPhysical > startPhysical) ? endPhysical - startPhysical : startPhysical - endPhysical);*/
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
void ofApp::setup() {

	//int index;
	//do
	//{
	//	cout << "---Menu for Pathfinding algorithms---" << endl;
	//	cout << "Enter 1 to start pathfinding on small graph" << endl;
	//	cout << "Enter 2 to start pathfinding on large graph" << endl;
	//	cout << "Enter 3 to start graphical demonstration of pathfinding with movement algorithms*" << endl;
	//	cout << "*after starting you cannot run path finding on small and large graphs untill you rerun the project" << endl;
	//	cin >> index;

	//	if (index == 1)
	//	{
	//		RunSmallGraph();
	//	}
	//	else if (index == 2)
	//	{
	//		RunLargeGraph();
	//	}

	//} while (index != 3);


	//mTileGraph = new TileGraph(Width, Height, 50, 50);
	//mTileGraph->GenerateObstacles(100);

	//mPlayer = new Boid(ofVec2f(10, 10));
	//mPlayer->mSteering = [this]()
	//{
	//	auto moveSteering = DynamicPathFollow::GetSteering(mPlayer->mKinematic, mPlayer->mTargets);
	//	auto rotationSteering = DynamicLookWhereYouAreGoing::GetSteering(mPlayer->mKinematic);
	//	return moveSteering + rotationSteering;
	//};


	SpawnBoids();
	mTileGraph = new TileGraph(Width, Height, 50, 50);
	mTileGraph->GenerateObstacles(100);

	CreateDT();

	CreateBT();

}

//--------------------------------------------------------------
void ofApp::update() {
	mPlayer->UpdateBoid(ofGetLastFrameTime());

	if (mF1)
	{
		mDigger->UpdateBoid(ofGetLastFrameTime());
		mDiggerManager->Update(ofGetLastFrameTime());
	}

	if (mF2)
	{
		mKiller->UpdateBoid(ofGetLastFrameTime());
		mKillerManager->Update(ofGetLastFrameTime());
	}

	float deadDistance = 76;

	if (mF2)
	{
		deadDistance = std::min(deadDistance, mPlayer->mKinematic.mPosition.distance(mKiller->mKinematic.mPosition));
	}
	if (mF3)
	{
		deadDistance = std::min(deadDistance, mPlayer->mKinematic.mPosition.distance(mDigger->mKinematic.mPosition));
	}

	if (deadDistance < 76)
	{
		delete mKiller;
		delete mDigger;
		delete mPlayer;

		SpawnBoids();

		((BT*)mKillerManager)->mBlackboard->ResetBlackboard();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	mTileGraph->DrawTileGraph();

	if (mF1)
	{
		mDigger->DrawBoid();
	}

	if (mF2)
	{
		mKiller->DrawBoid();
	}

	mPlayer->DrawBoid();
}

void ofApp::SpawnBoids()
{
	mKiller = new AIBoid(ofVec2f(990, 990), ofColor::darkBlue);
	mKiller->mTargets.pop();

	mDigger = new AIBoid(ofVec2f(990, 990), ofColor::aquamarine);
	mDigger->mTargets.pop();

	if (mDiggerManager)
	{
		((DTManager*)mDiggerManager)->SetCharacter(mDigger);
	}

	mPlayer = new Boid(ofVec2f(10, 10), ofColor::lawnGreen);
	std::function<SteeringOutputStructure()> steeringFunction = [this]()
	{
		auto moveSteering = DynamicPathFollow::GetSteering(mPlayer->mKinematic, mPlayer->mTargets);
		auto rotationSteering = DynamicLookWhereYouAreGoing::GetSteering(mPlayer->mKinematic);
		return moveSteering + rotationSteering;
	};

	mPlayer->changeSteeringFunction(steeringFunction);
}

void ofApp::SpawnCopyForGoap()
{
	mF3 = true;

	Goap* planner = new Goap(4, 3);

	DTDecisionNode* hasEnergy = new DTDecisionNode();
	DTDecisionNode* isPlayerInRange = new DTDecisionNode();
	DTDecisionNode* foundTarget = new DTDecisionNode();

	hasEnergy->isConditionTrue = [this]()
	{
		if (((AIBoid*)mDigger)->mEnergy >= 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	};

	isPlayerInRange->isConditionTrue = [this]()
	{

		float dist = mPlayer->mKinematic.mPosition.distance(mDigger->mKinematic.mPosition);
		float ang = std::abs(mPlayer->mKinematic.mPosition.angleRad(ofVec2f(1, 0)) - mDigger->mKinematic.mOrientation);
		float maxDistance = ((AIBoid*)mDigger)->mEnergy * 5.0f;
		float maxAngle = ((AIBoid*)mDigger)->mEnergy * 0.6f;

		
		if (dist < maxDistance && ang < maxAngle)
		{
			return true;
		}
		else
		{
			return false;
		}
	};

	foundTarget->isConditionTrue = [this]()
	{
		if (mDigger->mTargets.size() == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	};

	DTActionNode* reachPlayer = DTActionNode::Create<DTMoveToTarget>();
	DTActionNode* sleep = DTActionNode::Create<DTSleep>();
	DTActionNode* getPlayerTarget = DTActionNode::Create<DTActionTarget>();
	DTActionNode* wanderTarget = DTActionNode::Create<DTActionTarget>();

	((DTActionTarget*)getPlayerTarget->GetAction())->GetTarget = [this]()
	{
		auto source = mTileGraph->QauntizePosition(mDigger->mKinematic.mPosition);
		auto target = mTileGraph->QauntizePosition(mPlayer->mKinematic.mPosition);

		auto path = AStar::findPath(source, target, mTileGraph->mGraph);
		queue<KinematicStructure> localPath;
		for (int i = 1; i < path.size(); i++)
		{
			KinematicStructure dest;
			dest.mPosition = mTileGraph->LocalizedPosition(path[i]);
			localPath.push(dest);
		}
		while (!mDigger->mTargets.empty())
		{
			mDigger->mTargets.pop();
		}
		mDigger->SetTargets(localPath);
	};

	((DTActionTarget*)wanderTarget->GetAction())->GetTarget = [this]()
	{
		auto source = mTileGraph->QauntizePosition(mDigger->mKinematic.mPosition);
		auto target = mTileGraph->GetRandomMapLocation(source);

		auto path = AStar::findPath(source, target, mTileGraph->mGraph);
		queue<KinematicStructure> localPath;
		for (int i = 1; i < path.size(); i++)
		{
			KinematicStructure dest;
			dest.mPosition = mTileGraph->LocalizedPosition(path[i]);
			localPath.push(dest);
		}
		while (!mDigger->mTargets.empty())
		{
			mDigger->mTargets.pop();
		}
		mDigger->SetTargets(localPath);
	};

	planner->CreateNodeFromAction = [sleep, reachPlayer, getPlayerTarget, wanderTarget](unsigned int action)
	{
		switch (action)
		{
			case 0: return sleep; break;
			case 1: return getPlayerTarget; break;
			case 2: return wanderTarget; break;
			case 3: return reachPlayer; break;
		};
	};
	planner->CreateNodeFromAttribute = [hasEnergy, isPlayerInRange, foundTarget](unsigned int attribute)
	{
		switch (attribute)
		{
			case 0: return hasEnergy; break;
			case 1: return isPlayerInRange; break;
			case 2: return foundTarget; break;
		};
	};

	DT* dt = new DT(planner->CreateDecisionTreeBasedOnAttributes(((BT*)mKillerManager)->mBlackboard->GetRoute(), {0, 1, 2}));

	mDiggerManager = new DTManager();
	((DTManager*)mDigger)->SetAI(mDigger, dt);
}

void ofApp::CreateDT()
{
	DTDecisionNode* hasEnergy = new DTDecisionNode();
	DTDecisionNode* obstacleInRange = new DTDecisionNode();
	DTDecisionNode* isNearObstacle = new DTDecisionNode();


	DTActionNode* dig = DTActionNode::Create<DTDig>();
	DTActionNode* sleep = DTActionNode::Create<DTSleep>();
	DTActionNode* walk = DTActionNode::Create<DTWalk>();
	DTActionNode* wander = DTActionNode::Create<DTWander>();

	((DTDig*)dig->GetAction())->dig = [this]()
	{
		int node = mTileGraph->QauntizePosition(mDigger->mTargets.front().mPosition);
		TileGraph::Obstacle o;
		o.mSize = ofVec2f(50, 50);
		o.mCenter = ofVec2f(node / mTileGraph->mWidthCount, node % mTileGraph->mWidthCount);
		mTileGraph->RemoveObstacle(o);
	};

	hasEnergy->SetChildren(obstacleInRange, sleep);
	hasEnergy->isConditionTrue = [this]()
	{
		if (((AIBoid*)mDigger)->mEnergy >= 0)
		{
			//cout << "1. Has Energy " << ((AIBoid*)mDigger)->mEnergy << endl;
			return true;
		}

		else
		{
			//cout << "1. No Energy" << endl;
			return false;
		}
	};

	obstacleInRange->SetChildren(isNearObstacle, wander);
	obstacleInRange->isConditionTrue = [this]()
	{
		if (!mDigger->mTargets.empty())
		{
			//cout << "2. Has an obstacle target" << endl;
			return true;
		}
		for (auto obstacle : mTileGraph->mObstacles)
		{
			auto location = mTileGraph->LocalizedPosition(obstacle.mCenter.x * mTileGraph->mWidthCount + obstacle.mCenter.y);
			if (location.distance(mDigger->mKinematic.mPosition) > 100)
				continue;
			else
			{
				glm::vec2 loc = glm::vec2(location.x, location.y);
				if (std::abs(angle(loc, glm::vec2(1, 0)) - mDigger->mKinematic.mOrientation) < 30)
				{
					//while (!mBoid->mTargets.empty())
					//{
					//	mBoid->mTargets.pop();
					//}
					KinematicStructure target;
					target.mPosition = location;
					mDigger->mTargets.push(target);
					//cout << "2. Found a new obstacle target" << endl;
					return true;
				}
			}
		}
		//cout << "2. No obstacle target in Range" << endl;
		return false;
	};

	isNearObstacle->SetChildren(dig, walk);
	isNearObstacle->isConditionTrue = [this]()
	{
		if(mDigger->mTargets.front().mPosition.distance(mDigger->mKinematic.mPosition) < 50)
		{
			//cout << "3. Can access Obstacle" << endl;
			return true;
		}
		else
		{
			//cout << "3. Cannot access Obstacle" << endl;
			return false;
		}
	};

	DT* decisionTree = new DT(hasEnergy);

	mDiggerManager = new DTManager();
	((DTManager*)mDiggerManager)->SetAI(mDigger, decisionTree);

}

void ofApp::CreateBT()
{
	BTAction* sleep = new BTAction();
	sleep->ExecuteAction = [this](BTTick* iTick)
	{
		float &energy = ((AIBoid*)mKiller)->mEnergy;
		mKiller->mKinematic.mVelocity = ofVec2f(0, 0);

		if (energy > 90)
		{
			return Success;
		}
		else
		{
			energy += 0.166666667f * 7;
			return Running;
		}
	};

	sleep->OpenAction = [this](BTTick* iTick)
	{
		float dist = mPlayer->mKinematic.mPosition.distance(mKiller->mKinematic.mPosition);
		float ang = std::abs(mPlayer->mKinematic.mPosition.angleRad(ofVec2f(1, 0)) - mKiller->mKinematic.mOrientation);
		float maxDistance = ((AIBoid*)mKiller)->mEnergy * 5.0f;
		float maxAngle = ((AIBoid*)mKiller)->mEnergy * 0.6f;

		if (dist < maxDistance && ang < maxAngle)
		{
			iTick->mBlackboard->AddNodeToRoute(Tracer(0, { false, true, false }));
			iTick->mBlackboard->AddNodeToRoute(Tracer(0, { false, true, true }));
		}

		else
		{
			iTick->mBlackboard->AddNodeToRoute(Tracer(0, { false, false, false }));
			iTick->mBlackboard->AddNodeToRoute(Tracer(0, { false, false, true }));
		}
	};

	BTCondition* hasEnergy = new BTCondition();
	hasEnergy->AddChild(sleep);
	hasEnergy->ExecuteAction = [this, sleep](BTTick* iTick)
	{
		int energy = ((AIBoid*)mKiller)->mEnergy;
		bool *isNodeOpen = (bool*)iTick->mBlackboard->GetValueFromBlackboard(openString, iTick->mTree->mNodeID, sleep->GetNodeID());

		if (energy <= 0)
			return Success;
		else if (isNodeOpen && *isNodeOpen)
			return Success;
		else
			return Failure;
	};

	BTAction* getTaget = new BTAction();
	getTaget->ExecuteAction = [this](BTTick* iTick)
	{
		int *targetFound = (int*)iTick->mBlackboard->GetValueFromBlackboard(targetString, iTick->mTree->mNodeID);

		auto source = mTileGraph->QauntizePosition(mKiller->mKinematic.mPosition);
		int target = -1;

		//If the player is in range, then make him a target, no matter what.
		float dist = mPlayer->mKinematic.mPosition.distance(mKiller->mKinematic.mPosition);
		float ang = std::abs(mPlayer->mKinematic.mPosition.angleRad(ofVec2f(1, 0)) - mKiller->mKinematic.mOrientation);
		float maxDistance = ((AIBoid*)mKiller)->mEnergy * 5.0f;
		float maxAngle = ((AIBoid*)mKiller)->mEnergy * 0.6f;

		if (dist < maxDistance && ang < maxAngle)
		{
			float playerNode = mTileGraph->QauntizePosition(mPlayer->mKinematic.mPosition);
			if (!(targetFound && *targetFound == playerNode))
			{
				target = playerNode;
				iTick->mBlackboard->AddNodeToRoute(Tracer(1, { true, true, false }));
			}
		}

		//If the player is not in range, check if there was a a previous target already assigned.
		//If not, assign a new random target
		else if (!(targetFound && *targetFound != -1))
		{
			target = mTileGraph->GetRandomMapLocation(source);
			iTick->mBlackboard->AddNodeToRoute(Tracer(2, { true, false, false }));
		}

		//If there is a new target to chase, generate a path towards that target
		if (target != -1)
		{
			auto path = AStar::findPath(source, target, mTileGraph->mGraph);
			queue<KinematicStructure> localPath;
			for (int i = 1; i < path.size(); i++)
			{
				KinematicStructure dest;
				dest.mPosition = mTileGraph->LocalizedPosition(path[i]);
				localPath.push(dest);
			}
			while (!mKiller->mTargets.empty())
				mKiller->mTargets.pop();
			mKiller->SetTargets(localPath);

			iTick->mBlackboard->SetValueInBlackboard(targetString, new int(target), iTick->mTree->mNodeID);
		}

		return Success;
	};

	BTAction* walkToTarget = new BTAction();
	walkToTarget->ExecuteAction = [this](BTTick* iTick)
	{
		if (mKiller->mTargets.empty())
			iTick->mBlackboard->SetValueInBlackboard(targetString, new int(-1), iTick->mTree->mNodeID);
		else
		{
			auto s1 = DynamicPathFollow::GetSteering(mKiller->mKinematic, mKiller->mTargets);
			auto s2 = DynamicLookWhereYouAreGoing::GetSteering(mKiller->mKinematic);

			mKiller->mKinematic.UpdateDynamicMotion(s1 + s2, ofGetLastFrameTime());
			((AIBoid*)mKiller)->mEnergy -= 0.166666667f * 0.9f;

			iTick->mBlackboard->AddNodeToRoute(Tracer(3, { true, true, true }));
			iTick->mBlackboard->AddNodeToRoute(Tracer(3, { true, false, true }));
		}

		return Success;
	};

	BTSequencer* findAndKillSequence = new BTSequencer();
	findAndKillSequence->AddChild(getTaget);
	findAndKillSequence->AddChild(walkToTarget);

	BTSelector* canFindAndKill = new BTSelector();
	canFindAndKill->AddChild(hasEnergy);
	canFindAndKill->AddChild(findAndKillSequence);

	BTRepeatUntillFail* repeater = new BTRepeatUntillFail();
	repeater->AddChild(canFindAndKill);

	BTBlackboard* blackboard = new BTBlackboard();

	mKillerManager = new BT(0, repeater, blackboard);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == ofKey::OF_KEY_F1)
	{
		mF1 = !mF1;
	} else if (key == ofKey::OF_KEY_F2)
	{
		mF2 = !mF2;
	} else if (key == ofKey::OF_KEY_F3)
	{
		SpawnCopyForGoap();
	}
	if (key == ofKey::OF_KEY_F4)
	{
		ofExit(0);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	auto path = AStar::findPath(mTileGraph->QauntizePosition(mPlayer->mKinematic.mPosition), mTileGraph->QauntizePosition(ofVec2f(x, y)), mTileGraph->GetGraph());

	queue<KinematicStructure> localPath;
	for (auto node : path)
	{
		KinematicStructure destination;
		destination.mPosition = mTileGraph->LocalizedPosition(node);
		localPath.push(destination);
	}

	mPlayer->SetTargets(localPath);

	//if(currentAlgorithm == Algorithms::KinematicArrive || currentAlgorithm == Algorithms::DynamicArrive)
	//{
	//	sTarget.SetBoidLocation(ofVec2f(x, y));
	//	//kArriveAlgo.mTarget.mPosition = ofVec2f(x, y);
	//	isTimerActive = true;
	//}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
