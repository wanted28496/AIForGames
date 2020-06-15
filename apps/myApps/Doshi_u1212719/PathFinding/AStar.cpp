#include "AStar.h"
#include <iostream>
#include <ctime>
#include "NodeRecord.h"

AStar::AStar()
{
}


AStar::~AStar()
{
}

vector<int> AStar::findPath(int iStart, int iGoal, Graph & iGraph)
{
	clock_t startTime = clock();

	/// Open List
	priority_queue<NodeRecord*, vector<NodeRecord*>, NodeRecord> openList;
	vector<NodeRecord*> closeList;

	openList.push(new NodeRecord(iStart, nullptr, 0, iGraph.getHeuristic(iStart, iGoal)));
	int nodesVisited = 0;
	/// Iterate through the open list
	while (!openList.empty())
	{
		NodeRecord* currentNode = openList.top();
		openList.pop();

		if (currentNode->mNodeID == iGoal)
		{
			
			clock_t endTIme = clock();
			double d = double(endTIme - startTime) / CLOCKS_PER_SEC;
			cout << "Time taken for finding path: " << d << endl;
			cout << "Number of nodes visited for finding path: " << nodesVisited << endl;
			return currentNode->CreatePathFromDestination();
		}

		vector<DWEdge> outgoindEdges = iGraph.GetOutgoingEdgesFromSource(currentNode->mNodeID);

		vector<NodeRecord*>& openVector = Container(openList);
		for (auto edge : outgoindEdges)
		{
			/// Check if smaller path is found for nodes present in close list
			auto closedVectorNode = find_if(closeList.begin(), closeList.end(),
				[edge](NodeRecord* nr) { return (nr->mNodeID == edge.mSink); });
			if (closedVectorNode != closeList.end())
			{
				/// If smaller path found update the close list
				if ((*closedVectorNode)->mCostSoFar > currentNode->mCostSoFar + edge.mCost)
				{
					(*closedVectorNode)->mParent = currentNode;
					(*closedVectorNode)->ChangeValues(currentNode->mCostSoFar + edge.mCost, iGraph.getHeuristic(edge.mSink, iGoal));
					openList.push(*closedVectorNode);
					closeList.erase(closedVectorNode);
				}
				continue;
			}

			/// Check if smaller path is found for nodes present in open list
			auto openVectorNode = find_if(openVector.begin(), openVector.end(),
				[edge](NodeRecord* nr) { return (nr->mNodeID == edge.mSink); });
			if (openVectorNode != openVector.end())
			{
				if ((*openVectorNode)->mCostSoFar > currentNode->mCostSoFar + edge.mCost)
				{
					(*openVectorNode)->mParent = currentNode;
					(*openVectorNode)->ChangeValues(currentNode->mCostSoFar + edge.mCost, iGraph.getHeuristic(edge.mSink, iGoal));
					make_heap(const_cast<NodeRecord**>(&openList.top()), const_cast<NodeRecord**>(&openList.top() + openList.size()), NodeRecord());
				}
				continue;
			}

			openList.push(new NodeRecord(edge.mSink, currentNode, currentNode->mCostSoFar + edge.mCost, iGraph.getHeuristic(edge.mSink, iGoal)));
		}
		//cout << currentNode->mNodeID << " ";
		closeList.push_back(currentNode);
		
		nodesVisited++;
	}

	cout << "Time taken for finding path: " << double(clock() - startTime) / CLOCKS_PER_SEC << endl;
	cout << "Number of nodes visited for finding path: " << nodesVisited << endl;
	cout << "No Path found from source(" << iStart << ") to destination(" << iGoal << ")";
	return vector<int>();
}



//float PhysicalMemConsumption{
//	MEMORYSTATUSEX meminfo;
//}