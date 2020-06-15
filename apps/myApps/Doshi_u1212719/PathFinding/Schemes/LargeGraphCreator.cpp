#include "LargeGraphCreator.h"



LargeGraphCreator::LargeGraphCreator()
{
}

LargeGraphCreator::LargeGraphCreator(int iNodeCount) : 
	mGenerator(chrono::high_resolution_clock::now().time_since_epoch().count()),
	mDistribution(uniform_int_distribution<int>(0, 1000000)),
	mNodeCount(iNodeCount)
{
	GenerateGraph();
}


LargeGraphCreator::~LargeGraphCreator()
{
}

void LargeGraphCreator::UseManhattanDistance()
{
	mGraph.getHeuristic = [this](int iSource, int iDestination)
	{

		return abs((int)(mGraph.mNodes[iSource] / 1000) - (int)(mGraph.mNodes[iDestination] / 1000)) + \
					 abs((int)(mGraph.mNodes[iSource] % 1000) - (int)(mGraph.mNodes[iDestination] % 1000));
	};
}

void LargeGraphCreator::UseConstantDistance(int iDistance)
{
	constDis = iDistance;
	mGraph.getHeuristic = [this](int iSource, int iDestination)
	{
		return constDis;
	};
}

void LargeGraphCreator::UseEuclideanHuerisitc()
{
	mGraph.getHeuristic = [this](int iSource, int iDestination)
	{
		glm::vec2 a(mGraph.mNodes[iSource] / 1000, mGraph.mNodes[iSource] % 1000);
		glm::vec2 b(mGraph.mNodes[iDestination] / 1000, mGraph.mNodes[iDestination] % 1000);
		return (int)(glm::length(a - b));
	};
}

void LargeGraphCreator::UseDijkstraAlgorith()
{
	mGraph.getHeuristic = [this](int iSource, int iDestination)
	{
		return 0;
	};
}

void LargeGraphCreator::UseRandomHueristic(int iFirst, int iSecond)
{
	mGraph.getHeuristic = [this, iFirst, iSecond](int iSource, int iDestination)
	{
		return mDistribution(mGenerator) % (iSecond - iFirst) + iFirst;
	};
}

void LargeGraphCreator::UseRandomManhattenHueristic(int iFirst, int iSecond)
{
	mGraph.getHeuristic = [this, iFirst, iSecond](int iSource, int iDestination)
	{
		return mDistribution(mGenerator) % 100;
		return abs((int)(mGraph.mNodes[iSource] / 1000) - (int)(mGraph.mNodes[iDestination] / 1000)) + \
			abs((int)(mGraph.mNodes[iSource] % 1000) - (int)(mGraph.mNodes[iDestination] % 1000)) + \
			mDistribution(mGenerator) % (iSecond - iFirst) + iFirst;
	};
}

void LargeGraphCreator::GenerateGraph()
{
	vector<int> leftMostNodes;
	for (int i = 0; i < mNodeCount; i++)
	{
		leftMostNodes.push_back(0);
	}

	set<int> position;
	do
	{
		position.insert(mDistribution(mGenerator));
	} while (position.size() < mNodeCount);

	mGraph.mNodes = vector<int>(position.begin(), position.end());

	for (size_t i = 0; i < mGraph.mNodes.size(); i++)
	{
		int edgeCount = mDistribution(mGenerator) % 10 + 1;
		for (int j = 0; j < edgeCount; j++)
		{
			int destination = i;
			while (i == destination)
			{
				destination = mDistribution(mGenerator) % mNodeCount;
			}

			int cost = abs((int)(mGraph.mNodes[i] / 1000) - (int)(mGraph.mNodes[destination] / 1000)) +
				abs((int)(mGraph.mNodes[i] % 1000) - (int)(mGraph.mNodes[destination] % 1000)) + \
				mDistribution(mGenerator) % 30;

			leftMostNodes[destination] = 1;
			mGraph.mEdges.push_back(DWEdge(i, destination, cost));
		}
	}

	for (int i = 0; i < mNodeCount; i++)
	{
		int source = i;
		while (i == source)
		{
			source = mDistribution(mGenerator) % mNodeCount;
		}
		int cost = abs((int)(mGraph.mNodes[source] / 1000) - (int)(mGraph.mNodes[i] / 1000)) +
			abs((int)(mGraph.mNodes[source] % 1000) - (int)(mGraph.mNodes[i] % 1000)) + \
			mDistribution(mGenerator) % 30;
		mGraph.mEdges.push_back(DWEdge(source, i, cost));
	}

	UseDijkstraAlgorith();
}
