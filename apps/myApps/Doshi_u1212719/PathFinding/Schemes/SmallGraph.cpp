#include "SmallGraph.h"
//#include "ofMain.h"

SmallGraph::SmallGraph(int iNodeCount, vector<int> iMap) :
	mGenerator(chrono::high_resolution_clock::now().time_since_epoch().count()),
	mDistribution(uniform_int_distribution<int>(0, 1000000)),
	mNodeCount(iNodeCount),
	mMap(iMap)
{
	GenerateGraph();
}

SmallGraph::~SmallGraph()
{
}

void SmallGraph::UseManhattanDistance()
{
	mGraph.getHeuristic = [this](int iSource, int iDestination)
	{
		return mDistribution(mGenerator) % 10;
	};
}

void SmallGraph::UseConstantDistance(int iDistance)
{
	constDis = iDistance;
	mGraph.getHeuristic = [this](int iSource, int iDestination)
	{
		return constDis;
	};
}

void SmallGraph::UseOverEstimatedHueristic()
{
	mGraph.getHeuristic = [this](int iSource, int iDestination)
	{
		return 12;
	};
}

void SmallGraph::UseDijkstraAlgorith()
{
	mGraph.getHeuristic = [this](int iSource, int iDestination)
	{
		return 0;
	};
}



void SmallGraph::GenerateGraph()
{
	for (int i = 0; i < mNodeCount; i++)
	{
		for (int j = i + 1; j < mNodeCount; j++)
		{
			if (mMap[i * mNodeCount + j])
			{
				/// Bi directional Edges so add from Source to Sink and Sink to Source
				mGraph.mEdges.push_back(DWEdge(i, j, mMap[i * mNodeCount + j]));
				mGraph.mEdges.push_back(DWEdge(j, i, mMap[i * mNodeCount + j]));
			}
		}
	}

	mGraph.SortDirectWeightedEdges();
	/// For Dijkstra Algorithm set the Hueristic to 0 
	mGraph.getHeuristic = [this](int iSource, int iDestination)
	{
		return 0;
	};
}
