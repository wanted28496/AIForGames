#pragma once

#include <vector>
#include <functional>
#include <algorithm>
#include "ofApp.h"

using namespace std;

struct DWEdge
{
	int mCost;
	int mSource;
	int mSink;

	DWEdge(int iSource, int iSink, int iCost) :
		mSource(iSource), mSink(iSink), mCost(iCost) {}

	DWEdge() : DWEdge(0, 0, 0) {}

};

class Graph
{
	vector<DWEdge> mEdges;
	vector<int> mNodes;
public:
	Graph();
	Graph(vector<DWEdge> iEdges);
	~Graph();

	/// Creating a template function with functional library, 
	/// with int return type and two int as parameters 
	function<int(int, int)> getHeuristic;

	void SortDirectWeightedEdges();

	int GetCost(vector<int> iPath);

	vector<DWEdge> GetOutgoingEdgesFromSource(int iNode);

	friend class SmallGraph;
	friend class LargeGraphCreator;
	friend class TileGraph;
};

