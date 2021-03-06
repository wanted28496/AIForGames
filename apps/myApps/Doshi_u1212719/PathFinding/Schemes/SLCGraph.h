#include "Graph.h"
#include "ofApp.h"
#include <random>
#include <chrono> // USed for tracking time for each algorithm
#include <set>
#include <vector>

using namespace std;

class SLCGraph
{
	Graph mGraph;
	int mNodeCount;

	vector<int> mMap;

	default_random_engine mGenerator; /// for creating random numbers
	uniform_int_distribution<int> mDistribution; /// for creating random numbers in an uniform distribution

public:

	SLCGraph(int iNodeCount, vector<int> iMap);
	~SLCGraph();

	inline Graph& getGraph() { return mGraph; }
	int constDis = 0;
	void UseManhattanDistance();
	void UseConstantDistance(int iDistance);
	void UseOverEstimatedHueristic();
	void UseDijkstraAlgorith();

	void GenerateGraph();

};