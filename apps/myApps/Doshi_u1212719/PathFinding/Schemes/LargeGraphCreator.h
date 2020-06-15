#pragma once
#include "Graph.h"
#include "ofApp.h"
#include <random>
#include <chrono> // USed for tracking time for each algorithm
#include <set>
#include <vector>

class LargeGraphCreator
{

	Graph mGraph;
	int mNodeCount;
	default_random_engine mGenerator; /// for creating random numbers
	uniform_int_distribution<int> mDistribution; /// for creating random numbers in an uniform distribution

public:
	LargeGraphCreator();
	LargeGraphCreator(int iNodexCount);
	~LargeGraphCreator();

	inline Graph& getGraph() { return mGraph; }
	int constDis = 0;
	void UseManhattanDistance();
	void UseConstantDistance(int iDistance);
	void UseEuclideanHuerisitc();
	void UseDijkstraAlgorith();
	void UseRandomHueristic(int first, int second);
	void UseRandomManhattenHueristic(int first, int second);

	void GenerateGraph();
};

